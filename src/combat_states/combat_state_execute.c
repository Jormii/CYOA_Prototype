#include "ui.h"
#include "input.h"
#include "combat_damage.h"
#include "combat_interface.h"

State *combat_state_execute_update();
void combat_state_execute_on_enter(state_id_t previous_id);

State combat_state_execute = {
    .id = COMBAT_STATE_EXECUTE,
    .on_enter_cb = combat_state_execute_on_enter,
    .update_cb = combat_state_execute_update};

State *combat_state_execute_update()
{
    if (input_button_pressed(BUTTON_CROSS))
    {
        combat_engine_broadcast_engine_event(COMBAT_EVENT_ENGINE_END_OF_ROUND);
        combat_damage_reset();
        return &combat_state_start_of_round;
    }

    return STATE_SAME_STATE;
}

void combat_state_execute_on_enter(state_id_t previous_id)
{
    tb_clear(combat_interface.state_buffer, NULL);
    tb_clear(combat_interface.commands_buffer, NULL);

    combat_engine_execute_queue();
    tb_print(combat_interface.commands_buffer, 0x00FFFFFF, L"Executing skills... Press X to return to main state\n");
}