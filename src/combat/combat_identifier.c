#include "combat_team.h"
#include "combat_identifier.h"

bool_t combat_identifier_still_deployed(const CombatIdentifier *identifier)
{
    CombatUnit *current = combat_identifier_get_combat_unit(identifier);
    return current != NULL && current->unit->id == identifier->unit_id;
}

CombatUnit *combat_identifier_get_combat_unit(const CombatIdentifier *identifier)
{
    return combat_team_get_combat_unit(identifier->combat_team, identifier->unit_slot);
}