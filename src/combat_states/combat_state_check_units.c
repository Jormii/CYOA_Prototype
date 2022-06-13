#include "combat_interface.h"

State *combat_state_check_units_func();

State combat_state_check_units = {
    .id = COMBAT_STATE_CHECK_UNITS,
    .func_cb = combat_state_check_units_func,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_check_units_func()
{
    combat_interface.slot = 0;
    while (combat_interface.slot != MAX_UNITS_IN_COMBAT)
    {
        CombatUnit *cu = combat_team_get_combat_unit(
            &(combat_engine.players_team), combat_interface.slot);
        if (cu == NULL || cu->unit->hp == 0)
        {
            combat_interface.cursor = 0;
            return &combat_state_choose_unit;
        }
        else
        {
            combat_interface.slot += 1;
        }
    }

    combat_interface.cursor = 0;
    combat_interface.slot = 0;
    return &combat_state_ask_for_action;
}