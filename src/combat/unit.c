#include "unit.h"

void unit_default_initialization(Unit *unit)
{
    unit->id = UNDEFINED_UNIT_ID;
}

bool_t unit_is_valid(const Unit *unit)
{
    return unit->id != UNDEFINED_UNIT_ID;
}

stat_t unit_calculate_stat(const Unit *unit, Stat stat)
{
    // TODO
    switch (stat)
    {
    case STAT_SPEED:
        return unit->attributes[ATTR_AGILITY];
    default:
        break;
    }

    return 1;
}