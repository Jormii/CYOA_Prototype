#ifndef TEAM_H
#define TEAM_H

#include "unit.h"

#define MAX_UNITS_IN_TEAM 4

typedef struct Team_st
{
    Unit units[MAX_UNITS_IN_TEAM];
} Team;

void team_initialize(Team *team);

Unit *team_get_unit(Team *team, size_t index);

#endif