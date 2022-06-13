#include "combat_interface.h"

State *combat_state_start_of_round_func();

State combat_state_start_of_round = {
    .id = COMBAT_STATE_START_OF_ROUND,
    .func_cb = combat_state_start_of_round_func,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_start_of_round_func()
{
    ce_broadcast_engine_event(COMBAT_EVENT_START_OF_TURN);
    return &combat_state_check_units;
}