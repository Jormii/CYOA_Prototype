#include "die.h"
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
    if (!combat_identifier_still_deployed(&(dmg_instance->attacker)))
    {
        return;
    }

    CombatUnit *attacker = combat_identifier_get_combat_unit(&(dmg_instance->attacker));
    CombatUnit *defender = combat_identifier_get_combat_unit(&(dmg_instance->defender));
    if (defender == NULL)
    {
        return;
    }

    // TODO: Take into account modifiers?
    // TODO: Unit's levels
    // TODO: Random number in range [1-0.2, 1+0.2]
    stat_t attack = combat_unit_calculate_stat(attacker, STAT_DAMAGE);
    stat_t defense = combat_unit_calculate_stat(defender, STAT_ENDURANCE);

    u8_t attack_roll = roll_die(get_die(attack));
    u8_t defense_roll = roll_die(get_die(defense));

    float dmg = (float)dmg_instance->damage * (float)attack_roll / (float)defense_roll;
    dmg_instance->damage_dealt = (health_t)dmg; // TODO: Ceil?

    defender->unit->hp = umath_substract(defender->unit->hp, dmg_instance->damage_dealt);

    // Broadcast combat
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