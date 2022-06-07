#ifndef TEAM_H
#define TEAM_H

#include "unit.h"
#include "types.h"

#define MAX_UNITS_IN_TEAM 4
#define MAX_UNITS_IN_COMBAT 2
#define EMPTY_SLOT (MAX_UNITS_IN_TEAM + 1)

typedef u8_t slot_t;

typedef struct Team_st
{
    bool_t is_players_team;
    Unit units[MAX_UNITS_IN_TEAM];
    slot_t in_combat[MAX_UNITS_IN_COMBAT];
} Team;

void team_init(Team *team, bool_t is_players_team);
Unit *team_get_unit(Team *team, slot_t slot);

#endif