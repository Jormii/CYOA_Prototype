#include "ui.h"
#include "keyboard.h"
#include "game_state.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

// TODO: Move to its specific file
State game_state_keyboard = {
    .id = GAME_STATE_KEYBOARD,
    .on_enter_cb = NULL,
    .update_cb = keyboard_update};
