#include "combat_unit.h"

void combat_unit_initialize(CombatUnit *combat_unit)
{
    combat_unit->unit = NULL;
    combat_unit->died_this_round = FALSE;
}

bool_t combat_unit_tag_as_dead(CombatUnit *combat_unit) {
    // Returns if the unit was alive
    bool_t died_this_round = combat_unit->died_this_round;
    combat_unit->died_this_round = TRUE;

    return !died_this_round;
}

bool_t combat_unit_is_valid(const CombatUnit *combat_unit)
{
    return combat_unit->unit != NULL;
}