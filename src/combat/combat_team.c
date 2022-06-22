#include "combat_team.h"

void combat_team_initialize(CombatTeam *combat_team)
{
    team_initialize(&(combat_team->team));

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        combat_unit_initialize(combat_team->combat_units + slot);
    }
}

void combat_team_deploy_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot)
{
    if (combat_team_get_combat_unit(combat_team, slot) != NULL)
    {
        // A unit is being replaced
        combat_team_remove_from_combat(combat_team, slot);
    }

    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = unit;
    cu->died_this_round = FALSE;
    skillset_initialize(&(cu->skillset), unit->species->skillset_template);
}

void combat_team_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *cu = combat_team_get_combat_unit(combat_team, slot);
    if (cu == NULL)
    {
        return;
    }

    cu->unit = NULL;
    skillset_deinitialize(&(cu->skillset));
    fixed_list_clear(&(cu->special_conditions.fixed_list));

    // TODO: Notify skills this unit has been removed
}

CombatUnit *combat_team_get_combat_unit(CombatTeam *combat_team, combat_slot_t slot)
{
    if (slot >= MAX_UNITS_IN_COMBAT)
    {
        return NULL;
    }

    CombatUnit *cu = combat_team->combat_units + slot;
    if (combat_unit_is_valid(cu))
    {
        return cu;
    }
    else
    {
        return NULL;
    }
}

bool_t combat_team_unit_is_in_combat(CombatTeam *combat_team, size_t unit_id)
{
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(combat_team, slot);
        if (cu != NULL && cu->unit->id == unit_id)
        {
            return TRUE;
        }
    }

    return FALSE;
}

combat_slot_t combat_team_count_available_units(CombatTeam *combat_team)
{
    combat_slot_t count = 0;
    for (combat_slot_t i = 0; i < MAX_UNITS_IN_TEAM; ++i)
    {
        const Unit *unit = combat_team->team.units + i;
        count += !combat_team_unit_is_in_combat(combat_team, unit->id) && unit_is_alive(unit);
    }

    return count;
}

bool_t combat_team_format_combat_identifier(CombatTeam *combat_team, size_t unit_id, CombatIdentifier *out_identifier)
{
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(combat_team, slot);
        if (cu != NULL && cu->unit->id == unit_id)
        {
            out_identifier->unit_id = unit_id;
            out_identifier->unit_slot = slot;
            out_identifier->combat_team = combat_team;
            return TRUE;
        }
    }

    return FALSE;
}