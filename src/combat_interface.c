#include "ui.h"
#include "input.h"
#include "macros.h"
#include "combat_engine.h"
#include "combat_interface.h"

void combat_state_start();
void combat_state_turn_start();

void display_combat_state();
void display_combat_team(CombatTeam *combat_team);

// TODO: Delete this
size_t id = 0;

void init_unit(Unit *unit)
{
    unit->id = ++id;
    unit->name = L"NAME";
    unit->hp = 20;
    unit->stamina = 10;
    for (size_t i = 0; i < ATTR_COUNT; ++i)
    {
        unit->attributes[i] = 1;
    }
}

// END TODO

void combat_interface_initialize()
{
    combat_interface.state = COMBAT_STATE_START;
    combat_interface.state_cbs[COMBAT_STATE_START] = combat_state_start;
    combat_interface.state_cbs[COMBAT_STATE_TURN_START] = combat_state_turn_start;

    ce_initialize();

    // TODO: Remove this
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_TEAM; ++slot)
    {
        init_unit(combat_engine.players_team.team.units + slot);
        init_unit(combat_engine.enemy_team.team.units + slot);
    }
    // END TODO
}

void combat_interface_update()
{
    combat_interface.state_cbs[combat_interface.state]();
}

void combat_interface_start_combat()
{
    // Choose enemy units
    CombatTeam *combat_team = &(combat_engine.enemy_team);
    Team *team = &(combat_team->team);
    combat_slot_t limit = MIN(MAX_UNITS_IN_COMBAT, combat_team_count_available_units(combat_team));

    for (combat_slot_t slot = 0; slot < limit; ++slot)
    {
        ce_choose_unit(combat_team, team->units + slot, slot);
    }

    // Switch state
    combat_engine.in_combat = TRUE;
    combat_interface.state = COMBAT_STATE_START;
}

void combat_state_start()
{
    tb_clear(&(print_window.buffer), NULL);
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"--- Choose your starting units ---\n");

    // Handle input
    if (input_button_pressed(BUTTON_UP))
    {
        if (combat_interface.cursor != 0)
        {
            combat_interface.cursor -= 1;
        }
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
        if ((combat_interface.cursor + 1) != MAX_UNITS_IN_TEAM)
        {
            combat_interface.cursor += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CROSS))
    {
        Unit *unit_highlighted = team_get_unit(&(combat_engine.players_team.team), combat_interface.cursor);
        if (unit_highlighted != NULL && !combat_team_unit_is_in_combat(&(combat_engine.players_team), unit_highlighted->id))
        {
            ce_choose_unit(&(combat_engine.players_team), unit_highlighted, combat_interface.units_chosen);
            combat_interface.units_chosen += 1;

            combat_slot_t available = combat_team_count_available_units(&(combat_engine.players_team));
            if (available == 0 || combat_interface.units_chosen == MAX_UNITS_IN_COMBAT)
            {
                combat_interface.state = COMBAT_STATE_TURN_START;
                return; // Force return. Else the following code will break
            }
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (combat_interface.units_chosen != 0)
        {
            combat_interface.units_chosen -= 1;
            ce_remove_from_combat(&(combat_engine.players_team), combat_interface.units_chosen);
        }
    }

    // Print all player's units
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_TEAM; ++slot)
    {
        rgb_t color = 0x00FFFFFF;
        if (slot == combat_interface.cursor)
        {
            color = 0x0000FFFF;
        }

        // Highlight if chosen
        const Unit *unit = team_get_unit(&(combat_engine.players_team.team), slot);
        if (unit != NULL)
        {
            if (combat_team_unit_is_in_combat(&(combat_engine.players_team), unit->id))
            {
                tb_print(&(print_window.buffer), color, L"+ ");
            }
            else
            {
                tb_print(&(print_window.buffer), color, L"- ");
            }

            tb_printf(&(print_window.buffer), color, L"%ls (%u)\n",
                      unit->name, unit->id);
        }
        else
        {
            tb_print(&(print_window.buffer), color, L"----------\n");
        }
    }
}

void combat_state_turn_start()
{
    tb_clear(&(print_window.buffer), NULL);
    tb_clear(&(commands_window.buffer), NULL);

    display_combat_state();
}

void display_combat_state()
{
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"--- YOUR UNITS ---\n");
    display_combat_team(&(combat_engine.players_team));

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n--- ENEMY UNITS ---\n");
    display_combat_team(&(combat_engine.enemy_team));
}

void display_combat_team(CombatTeam *combat_team)
{
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *combat_unit = combat_team_get_combat_unit(combat_team, slot);
        if (combat_unit != NULL)
        {
            const Unit *unit = combat_unit->unit;
            tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%u :: %ls (%u)\n",
                      slot, unit->name, unit->id);
        }
    }
}