#include "dynamic_list.h"
#include "combat_damage.h"
#include "combat_engine.h"
#include "unsigned_math.h"

#define DEFAULT_QUEUE_SIZE 2

DynamicList dmg_instances_queue;

void combat_damage_initialize()
{
    dynamic_list_init(&dmg_instances_queue, DEFAULT_QUEUE_SIZE, 1,
                      sizeof(DmgCalcInstance));
}

void combat_damage_declare_attack(const CombatIdentifier *attacker, const CombatIdentifier *defender)
{
    DmgCalcInstance instance = {
        .attacker = *attacker,
        .defender = *defender};

    dynamic_list_append(&dmg_instances_queue, (byte_t *)&instance);
    combat_engine_broadcast_engine_event(COMBAT_EVENT_ENGINE_ATTACK_DECLARATION);
}

DmgCalcInstance *combat_damage_peek_queue()
{
    FixedList *l = &(dmg_instances_queue.fixed_list);
    if (l->length == 0)
    {
        return NULL;
    }

    return (DmgCalcInstance *)fixed_list_get(l, l->length - 1);
}

void combat_damage_perform()
{
    DmgCalcInstance *instance = combat_damage_peek_queue();
    if (instance == NULL)
    {
        return;
    }

    health_t damage = 1;
    if (combat_identifier_still_deployed(&(instance->attacker)))
    {
        // TODO
        // CombatUnit *attacker = combat_identifier_get_combat_unit(&(instance->attacker));
        CombatUnit *defender = combat_identifier_get_combat_unit(&(instance->defender));
        if (defender != NULL)
        {
            defender->unit->hp = umath_substract(defender->unit->hp, damage);
        }
    }

    fixed_list_remove(&(dmg_instances_queue.fixed_list),
                      dmg_instances_queue.fixed_list.length - 1);
}