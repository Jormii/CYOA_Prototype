#ifndef COMBAT_ENGINE_H
#define COMBAT_ENGINE_H

#include "team.h"
#include "unit.h"
#include "types.h"
#include "fixed_list.h"
#include "dynamic_list.h"
#include "combat_damage.h"

#define MAX_UNITS_IN_COMBAT 2

typedef struct CombatUnit_st
{
    Unit *unit;
    bool_t slot_occupied;
    SkillSet skillset;
    // TODO: Special conditions
} CombatUnit;

typedef struct CombatTeam_st
{
    Team team;
    bool_t is_players_team;
    CombatUnit combat_units[MAX_UNITS_IN_COMBAT];
} CombatTeam;

CombatUnit *combat_team_get_combat_unit(CombatTeam *combat_team, combat_slot_t slot);
combat_slot_t combat_team_count_available_units(const CombatTeam *combat_team);
bool_t combat_team_unit_is_in_combat(const CombatTeam *combat_team, size_t unit_id);

struct
{
    bool_t in_combat;
    CombatTeam players_team;
    CombatTeam enemy_team;
    FixedList active_commands_queue;
    DynamicList passive_commands_queue;
} combat_engine;

void ce_initialize();

void ce_choose_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot);
void ce_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot);

void ce_broadcast_engine_event(CombatEvent event);
void ce_broadcast_event(CombatEventSource *source);

void ce_add_active_to_queue(const ActiveSkillCommand *command);
void ce_remove_queue_tail();
void ce_execute_queue();

#endif