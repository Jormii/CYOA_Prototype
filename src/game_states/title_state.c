#include "state.h"
#include "game_state.h"
#include "title_screen.h"

State *game_state_title_update();

State game_state_title = {
    .id = GAME_STATE_TITLE,
    .on_enter_cb = NULL,
    .update_cb = game_state_title_update};

State *game_state_title_update()
{
    title_screen_update();
    return STATE_SAME_STATE;
}