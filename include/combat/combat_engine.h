#ifndef COMBAT_ENGINE_H
#define COMBAT_ENGINE_H

#include "team.h"
#include "unit.h"
#include "types.h"

#define MAX_UNITS_IN_COMBAT 2

typedef u8_t combat_slot_t;

typedef struct CombatUnit_st
{
    Unit *unit;
    bool_t slot_occupied;
    // Skillset and special conditions
} CombatUnit;

typedef struct CombatTeam_st
{
    Team team;
    bool_t is_players_team;
    CombatUnit combat_units[MAX_UNITS_IN_COMBAT];
} CombatTeam;

CombatUnit *combat_team_get_combat_unit(CombatTeam *combat_team, combat_slot_t slot);
bool_t combat_team_unit_is_in_combat(const CombatTeam *combat_team, size_t unit_id);

struct
{
    bool_t in_combat;
    CombatTeam players_team;
    CombatTeam enemy_team;
} combat_engine;

void ce_initialize();
void ce_choose_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot);
void ce_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot);

#endif