#include "state.h"
#include "game_state.h"
#include "creature_screen.h"

State *game_state_creature_update();

State game_state_creature = {
    .id = GAME_STATE_CREATURE,
    .on_enter_cb = NULL,
    .update_cb = game_state_creature_update};

State *game_state_creature_update()
{
    creature_screen_update();
    return STATE_SAME_STATE;
}