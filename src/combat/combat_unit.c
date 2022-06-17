#include "combat_unit.h"

void combat_unit_initialize(CombatUnit *combat_unit)
{
    combat_unit->unit = NULL;
}

bool_t combat_unit_is_valid(const CombatUnit *combat_unit)
{
    return combat_unit->unit != NULL;
}