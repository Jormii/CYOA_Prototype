#include "combat_interface.h"

State *combat_state_start_of_round_update();

State combat_state_start_of_round = {
    .id = COMBAT_STATE_START_OF_ROUND,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .update_cb = combat_state_start_of_round_update};

State *combat_state_start_of_round_update()
{
    combat_engine_broadcast_engine_event(COMBAT_EVENT_ENGINE_START_OF_ROUND);
    return &combat_state_check_units;
}