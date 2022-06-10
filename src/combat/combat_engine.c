#include "utils.h"
#include "combat_engine.h"

void combat_team_initialize(CombatTeam *combat_team, bool_t is_players_team);

CombatUnit *combat_team_get_combat_unit(CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    if (cu->slot_occupied)
    {
        return cu;
    }
    else
    {
        return NULL;
    }
}

combat_slot_t combat_team_count_available_units(const CombatTeam *combat_team)
{
    combat_slot_t count = 0;
    for (combat_slot_t i = 0; i < MAX_UNITS_IN_TEAM; ++i)
    {
        const Unit *unit = combat_team->team.units + i;
        bool_t available = unit_is_valid(unit) && unit->hp > 0 && !combat_team_unit_is_in_combat(combat_team, unit->id);
        count += available;
    }

    return count;
}

bool_t combat_team_unit_is_in_combat(const CombatTeam *combat_team, size_t unit_id)
{
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team->combat_units + slot;
        if (cu->slot_occupied && cu->unit->id == unit_id)
        {
            return TRUE;
        }
    }

    return FALSE;
}

void ce_initialize()
{
    combat_engine.in_combat = FALSE;
    combat_team_initialize(&(combat_engine.players_team), TRUE);
    combat_team_initialize(&(combat_engine.enemy_team), FALSE);
    fixed_list_init(
        &(combat_engine.active_commands_queue), 2 * MAX_UNITS_IN_COMBAT, sizeof(ActiveSkillCommand));
}

void ce_choose_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = unit;
    cu->slot_occupied = TRUE;
}

void ce_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = NULL;
    cu->slot_occupied = FALSE;
}

void ce_add_active_to_queue(const ActiveSkillCommand *command)
{
    fixed_list_append(&(combat_engine.active_commands_queue), (byte_t *)command);
}

void ce_execute_queue()
{
    // TODO: Sort queue

    ActiveSkillCommand command;
    FixedList *queue = &(combat_engine.active_commands_queue);
    while (queue->length != 0)
    {
        // Pop
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, sizeof(ActiveSkillCommand));
        fixed_list_remove(queue, 0);

        // Execute
        command.active->execute_cb(&command);
    }
}

void combat_team_initialize(CombatTeam *combat_team, bool_t is_players_team)
{
    team_default_initialization(&(combat_team->team));
    combat_team->is_players_team = is_players_team;

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        CombatUnit *cu = combat_team->combat_units + slot;
        cu->unit = NULL;
        cu->slot_occupied = FALSE;
    }
}