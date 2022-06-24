#include "dynamic_list.h"
#include "combat_damage.h"
#include "combat_engine.h"
#include "unsigned_math.h"

#define DEFAULT_QUEUE_SIZE (2 * MAX_UNITS_IN_COMBAT)

DynamicList dmg_instances;
size_t dmg_instance_id_counter = 0;

void combat_damage_initialize()
{
    dynamic_list_init(&dmg_instances, DEFAULT_QUEUE_SIZE, 1,
                      sizeof(DmgCalcInstance));
}

DmgCalcInstance *combat_damage_declare_attack(const CombatIdentifier *attacker, const CombatIdentifier *defender, SkillCommand *cause)
{
    DmgCalcInstance instance = {
        .id = dmg_instance_id_counter++,
        .damage = 1,
        .cause = cause,
        .attacker = *attacker,
        .defender = *defender};

    dynamic_list_append(&dmg_instances, (byte_t *)&instance);

    SkillCommand combat_command;
    combat_engine_format_passive_command(
        cause->skill, attacker, defender, COMBAT_EVENT_UNIT_ATTACK_DECLARATION, cause, instance.id,
        &combat_command);
    combat_engine_broadcast_event(&combat_command);

    return combat_damage_get_instance(instance.id);
}

void combat_damage_perform(DmgCalcInstance *dmg_instance)
{
    health_t damage = dmg_instance->damage;
    if (combat_identifier_still_deployed(&(dmg_instance->attacker)))
    {
        // TODO: Implement damage function
        // CombatUnit *attacker = combat_identifier_get_combat_unit(&(dmg_instance->attacker));
        CombatUnit *defender = combat_identifier_get_combat_unit(&(dmg_instance->defender));
        if (defender != NULL)
        {
            defender->unit->hp = umath_substract(defender->unit->hp, damage);
        }
    }

    SkillCommand command_event;
    combat_engine_format_passive_command(
        dmg_instance->cause->skill, &(dmg_instance->attacker), &(dmg_instance->defender),
        COMBAT_EVENT_UNIT_ATTACK_COMPLETION, dmg_instance->cause, dmg_instance->id,
        &command_event);
    combat_engine_broadcast_event(&command_event);
}

void combat_damage_reset()
{
    fixed_list_clear(&(dmg_instances.fixed_list));
}

DmgCalcInstance *combat_damage_get_instance(size_t dmg_instance_id)
{
    FixedList *list = &(dmg_instances.fixed_list);
    for (size_t i = 0; i < list->length; ++i)
    {
        DmgCalcInstance *dmg_inst = (DmgCalcInstance *)fixed_list_get(list, i);
        if (dmg_inst->id == dmg_instance_id)
        {
            return dmg_inst;
        }
    }

    return NULL;
}