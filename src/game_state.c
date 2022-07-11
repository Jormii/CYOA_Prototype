#include "ui.h"
#include "keyboard.h"
#include "game_state.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

// TODO: Move to its specific file

State game_state_story = {
    .id = GAME_STATE_STORY,
    .on_enter_cb = NULL,
    .update_cb = cyoa_interface_update};

void game_state_combat_on_enter(state_id_t previous_id);
State game_state_combat = {
    .id = GAME_STATE_COMBAT,
    .on_enter_cb = NULL,
    .update_cb = combat_interface_update};

State game_state_keyboard = {
    .id = GAME_STATE_KEYBOARD,
    .on_enter_cb = NULL,
    .update_cb = keyboard_update};

void game_state_combat_on_enter(state_id_t previous_id)
{
    combat_interface_start_combat();
}
