#include "combat_interface.h"

State *combat_state_end_of_combat_update();

State combat_state_end_of_combat = {
    .id = COMBAT_STATE_END_OF_COMBAT,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .update_cb = combat_state_end_of_combat_update};

State *combat_state_end_of_combat_update()
{
    return STATE_SAME_STATE;
}