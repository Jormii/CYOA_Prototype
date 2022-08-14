#include "input.h"
#include "state.h"
#include "game_state.h"
#include "combat_screen.h"
#include "combat_interface.h"

void game_state_combat_on_enter(state_id_t previous_id);
State *game_state_combat_update();

State game_state_combat = {
    .id = GAME_STATE_COMBAT,
    .on_enter_cb = game_state_combat_on_enter,
    .update_cb = game_state_combat_update};

void game_state_combat_on_enter(state_id_t previous_id)
{
    combat_interface_start_combat();
}

State *game_state_combat_update()
{
    combat_screen_update();

    if (input_button_pressed(BUTTON_TRIANGLE))
    {
        // Swap window being displayed
        combat_interface.displaying_state = !combat_interface.displaying_state;
    }
    return combat_interface_update();
}