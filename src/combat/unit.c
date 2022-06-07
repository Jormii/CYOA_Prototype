#include "unit.h"

void unit_default_initialization(Unit *unit)
{
    unit->id = UNDEFINED_UNIT_ID;
}

bool_t unit_is_valid(Unit *unit)
{
    return unit->id != UNDEFINED_UNIT_ID;
}