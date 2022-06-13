#include "combat_interface.h"

State *combat_state_end_of_combat_func();

State combat_state_end_of_combat = {
    .id = COMBAT_STATE_END_OF_COMBAT,
    .func_cb = combat_state_end_of_combat_func,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_end_of_combat_func()
{
    return STATE_SAME_STATE;
}