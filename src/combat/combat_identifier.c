#include "combat_team.h"
#include "combat_identifier.h"

void combat_identifier_copy(const CombatIdentifier *src, CombatIdentifier *dst)
{
    dst->unit_id = src->unit_id;
    dst->unit_slot = src->unit_slot;
    dst->combat_team = src->combat_team;
}

bool_t combat_identifier_are_same_unit(const CombatIdentifier *first, const CombatIdentifier *second)
{
    return first->unit_id == second->unit_id;
}

bool_t combat_identifier_still_deployed(const CombatIdentifier *identifier)
{
    CombatUnit *current = combat_identifier_get_combat_unit(identifier);
    return current != NULL && current->unit->id == identifier->unit_id;
}

CombatUnit *combat_identifier_get_combat_unit(const CombatIdentifier *identifier)
{
    return combat_team_get_combat_unit(identifier->combat_team, identifier->unit_slot);
}