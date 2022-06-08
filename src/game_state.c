#include "game_state.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

GameStateIdentifier current_state;
GameState states[GAME_STATE_COUNT];

void game_state_none() {}

void game_state_initialize()
{
    current_state = GAME_STATE_NONE;

    states[GAME_STATE_NONE].update_cb = game_state_none;
    states[GAME_STATE_NONE].on_enter_cb = game_state_none;

    states[GAME_STATE_STORY].update_cb = cyoa_interface_update;
    states[GAME_STATE_STORY].on_enter_cb = NULL;

    states[GAME_STATE_COMBAT].update_cb = combat_interface_update;
    states[GAME_STATE_COMBAT].on_enter_cb = combat_interface_start_combat;
}

void game_state_update()
{
    states[current_state].update_cb();
}

void game_state_switch_to(GameStateIdentifier state_id)
{
    current_state = state_id;

    GameStateCb_fp on_enter = states[current_state].on_enter_cb;
    if (on_enter != NULL)
    {
        on_enter();
    }
}