#include "dynamic_list.h"
#include "combat_damage.h"
#include "combat_engine.h"
#include "unsigned_math.h"

#define DEFAULT_QUEUE_SIZE 2

DynamicList dmg_instances_queue;

void ce_damage_initialize()
{
    dynamic_list_init(&dmg_instances_queue, DEFAULT_QUEUE_SIZE, 1,
                      sizeof(DmgCalcInstance));
}

void ce_damage_declare_attack(CombatDescriptor *attacker, CombatDescriptor *defender)
{
    DmgCalcInstance instance = {
        .attacker = attacker,
        .defender = defender};

    dynamic_list_append(&dmg_instances_queue, (byte_t *)&instance);
}

DmgCalcInstance *ce_damage_current()
{
    FixedList *l = &(dmg_instances_queue.fixed_list);
    if (l->length == 0)
    {
        return NULL;
    }

    return (DmgCalcInstance *)fixed_list_get(l, l->length - 1);
}

void ce_damage_perform()
{
    DmgCalcInstance *instance = ce_damage_current();
    if (instance == NULL)
    {
        return;
    }

    health_t damage = 1;
    CombatUnit *defender = combat_team_get_combat_unit(instance->defender->combat_team, instance->defender->unit_slot);
    defender->unit->hp = umath_substract(defender->unit->hp, damage);

    fixed_list_remove(&(dmg_instances_queue.fixed_list),
                      dmg_instances_queue.fixed_list.length - 1);
}