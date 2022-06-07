#include "team.h"

void team_default_initialization(Team *team)
{
    for (size_t i = 0; i < MAX_UNITS_IN_TEAM; ++i)
    {
        unit_default_initialization(team->units + i);
    }
}

Unit *team_get_unit(Team *team, size_t index)
{
    Unit *unit = team->units + index;
    if (unit_is_valid(unit))
    {
        return unit;
    }
    else
    {
        return NULL;
    }
}