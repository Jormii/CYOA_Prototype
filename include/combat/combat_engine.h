#ifndef COMBAT_ENGINE_H
#define COMBAT_ENGINE_H

#include "combat_team.h"
#include "dynamic_list.h"

struct
{
    bool_t in_combat;
    CombatTeam players_team;
    CombatTeam enemy_team;
    DynamicList skills_queue;
} combat_engine;

void combat_engine_initialize();

void combat_engine_broadcast_engine_event(CombatEvent event);
void combat_engine_broadcast_event(CombatEventSource *source);

void combat_engine_add_active_to_queue(const SkillCommand *command);
void combat_engine_remove_queue_tail();
void combat_engine_execute_queue();

#endif