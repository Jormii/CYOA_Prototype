#include "state.h"
#include "game_state.h"
#include "story_screen.h"
#include "cyoa_interface.h"

State *game_state_story_update();

State game_state_story = {
    .id = GAME_STATE_STORY,
    .on_enter_cb = NULL,
    .update_cb = game_state_story_update};

State *game_state_story_update()
{
    story_screen_update();
    return cyoa_interface_update();
}