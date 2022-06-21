#ifndef COMBAT_TEAM_H
#define COMBAT_TEAM_H

#include "team.h"
#include "combat_unit.h"

#define MAX_UNITS_IN_COMBAT 2

typedef struct CombatTeam_st
{
    Team team;
    CombatUnit combat_units[MAX_UNITS_IN_COMBAT];
} CombatTeam;

void combat_team_initialize(CombatTeam *combat_team);

void combat_team_deploy_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot);
void combat_team_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot);

CombatUnit *combat_team_get_combat_unit(CombatTeam *combat_team, combat_slot_t slot);
bool_t combat_team_unit_is_in_combat(CombatTeam *combat_team, size_t unit_id);
combat_slot_t combat_team_count_available_units(CombatTeam *combat_team);

bool_t combat_team_format_combat_identifier(CombatTeam *combat_team, size_t unit_id, CombatIdentifier *out_identifier);

#endif