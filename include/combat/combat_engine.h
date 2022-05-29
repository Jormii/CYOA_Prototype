#ifndef COMBAT_ENGINE_H
#define COMBAT_ENGINE_H

#include "team.h"
#include "fixed_list.h"
#include "active_skill.h"
#include "combat_event.h"
#include "dynamic_list.h"

#define COMMAND_QUEUE_LENGTH (2 * MAX_UNITS_IN_COMBAT)
#define PASSIVE_COMMAND_QUEUE_LENGTH (2 * COMMAND_QUEUE_LENGTH)

struct
{
    bool_t in_combat;
    Team players_team;
    Team enemy_team;
    FixedList active_commands;
    DynamicList passive_commands;
} combat_engine;

void ce_init();
void ce_start_combat();
void ce_combat_loop();
void ce_push_active_command(const ActiveSkillCommand *command);
void ce_sort_command_queue();
void ce_execute_command_queue();
void ce_update_combat_state();
void ce_engine_broadcast(CombatEvent event);
void ce_broadcast_event(CombatEventSource *source);
void ce_broadcast_event_to_team(CombatEventSource *source, Team *team);
void ce_broadcast_event_to_slot(CombatEventSource *source, Team *team, slot_t slot);


extern void ce_on_event(CombatEventSource *source);
extern ActiveSkill *ce_ask_for_skill(const Unit *unit);
extern void ce_ask_for_skills_target(const ActiveSkill *skill, UnitSource *target_out);

#endif