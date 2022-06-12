#include "team.h"

void ce_init_team(Team *team, bool_t is_players_team)
{
    team->is_players_team = is_players_team;

    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        team->in_combat[slot] = EMPTY_SLOT;
    }
}

Unit *team_get_unit(Team *team, slot_t slot)
{
    size_t unit_index = team->in_combat[slot];
    if (unit_index == EMPTY_SLOT)
    {
        return NULL;
    }
    else
    {
        return team->units + unit_index;
    }
}