#include "ui.h"
#include "input.h"
#include "combat_interface.h"

State *combat_state_choose_unit_func();
void combat_state_choose_unit_on_enter(state_id_t previous_id);

void display_players_units();
extern void display_combat_state(); // TODO: Move to some .h

State combat_state_choose_unit = {
    .id = COMBAT_STATE_CHOOSE_UNIT,
    .func_cb = combat_state_choose_unit_func,
    .on_enter_cb = combat_state_choose_unit_on_enter,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_choose_unit_func()
{
    // Update interface
    tb_clear(&(commands_window.buffer), NULL);
    tb_print(&(commands_window.buffer), 0x00FFFFFF, L"--- Choose your starting units ---\n");
    display_players_units();

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
        if (unit_highlighted != NULL && unit_highlighted->hp != 0 && !combat_team_unit_is_in_combat(&(combat_engine.players_team), unit_highlighted->id))
        {
            combat_team_deploy_unit(
                &(combat_engine.players_team), unit_highlighted, combat_interface.slot);

            if (combat_engine.in_combat)
            {
                return &combat_state_check_units;
            }
            else
            {
                // Flag CombatEngine::in_combat is FALSE when choosing starting units
                combat_interface.slot += 1;
                return &combat_state_start_of_combat;
            }
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        // Cancelling is only allowed at start of combat => Flag CombatEngine::in_combat is FALSE
        if (!combat_engine.in_combat && combat_interface.slot != 0)
        {
            combat_interface.slot -= 1;
            combat_team_remove_from_combat(
                &(combat_engine.players_team), combat_interface.slot);
            return &combat_state_start_of_combat;
        }
    }

    return STATE_SAME_STATE;
}

void combat_state_choose_unit_on_enter(state_id_t previous_id)
{
    tb_clear(&(print_window.buffer), NULL);
    display_combat_state();
}

void display_players_units()
{
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
                tb_print(&(commands_window.buffer), color, L"+ ");
            }
            else
            {
                tb_print(&(commands_window.buffer), color, L"- ");
            }

            tb_printf(&(commands_window.buffer), color, L"%ls (%u)\n",
                      unit->name, unit->id);
        }
        else
        {
            tb_print(&(commands_window.buffer), color, L"----------\n");
        }
    }
}