#include "dynamic_list.h"
#include "combat_engine.h"
#include "damage_calculator.h"

#define DEFAULT_QUEUE_SIZE 2

DynamicList instances_queue;

void send_event(CombatEvent event, const UnitSource *who);

void damage_calculator_init()
{
    dynamic_list_init(&instances_queue, DEFAULT_QUEUE_SIZE,
                      DYNAMIC_LIST_INCREMENT_DOUBLE, sizeof(DmgCalcInstance));
}

DmgCalcInstance *damage_calculator_get()
{
    if (instances_queue.fixed_list.length == 0)
    {
        return NULL;
    }

    return (DmgCalcInstance *)fixed_list_get(&(instances_queue.fixed_list),
                                             instances_queue.fixed_list.length - 1);
}

void damage_calculator_declare(UnitSource *attacker, UnitSource *defender,
                               bool_t trigger_declaration)
{
    DmgCalcInstance instance = {
        .attacker = attacker,
        .defender = defender,
        .multiplier = 1.0f};
    dynamic_list_append(&instances_queue, (byte_t *)(&instance));

    if (trigger_declaration)
    {
        send_event(EVENT_COMBAT_DECLARATION, attacker);
    }
}

void damage_calculator_perform()
{
    DmgCalcInstance *instance = damage_calculator_get();
    if (instance == NULL)
    {
        return;
    }

    send_event(EVENT_CALCULATE_OFFENSE, instance->attacker);

    health_t damage = (health_t)(1.0f * instance->multiplier);
    Unit *defender = source_get_unit(instance->defender);
    defender->hp -= damage;

    fixed_list_remove(&(instances_queue.fixed_list),
                      instances_queue.fixed_list.length - 1);
}

void send_event(CombatEvent event, const UnitSource *who)
{
    CombatEventSource source = {
        .event = event,
        .caused_by_engine = FALSE,
        .responsible = {
            .unit_id = who->unit_id,
            .team = who->team,
            .slot = who->slot}};
    ce_broadcast_event(&source);
}