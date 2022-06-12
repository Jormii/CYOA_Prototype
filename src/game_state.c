#include "game_state.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

typedef enum GameStateIdentifier_en
{
    GAME_STATE_STORY,
    GAME_STATE_COMBAT
} GameStateIdentifier;

StateManager game_state_manager;
State game_state_story;
State game_state_combat;

void game_state_initialize()
{
    game_state_manager.state = &game_state_combat;

    game_state_story.id = GAME_STATE_STORY;
    game_state_story.func_cb = cyoa_interface_update;
    game_state_story.on_enter_cb = STATE_CALLBACK_NONE;
    game_state_story.on_exit_cb = STATE_CALLBACK_NONE;

    game_state_combat.id = GAME_STATE_COMBAT;
    game_state_combat.func_cb = combat_interface_update;
    game_state_combat.on_enter_cb = STATE_CALLBACK_NONE;
    game_state_combat.on_exit_cb = STATE_CALLBACK_NONE;
}

void game_state_update()
{
    state_manager_execute(&game_state_manager);
}