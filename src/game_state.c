#include "keyboard.h"
#include "game_state.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

typedef enum GameStateIdentifier_en
{
    GAME_STATE_STORY,
    GAME_STATE_COMBAT,
    GAME_STATE_KEYBOARD
} GameStateIdentifier;

StateManager game_state_manager;
State game_state_story;
State game_state_combat;
State game_state_keyboard;

void game_state_combat_on_enter(state_id_t previous_id);

void game_state_initialize(State *starting_state)
{
    game_state_story.id = GAME_STATE_STORY;
    game_state_story.on_enter_cb = STATE_CALLBACK_NONE;
    game_state_story.update_cb = cyoa_interface_update;

    game_state_combat.id = GAME_STATE_COMBAT;
    game_state_combat.on_enter_cb = game_state_combat_on_enter;
    game_state_combat.update_cb = combat_interface_update;

    game_state_keyboard.id = GAME_STATE_KEYBOARD;
    game_state_keyboard.on_enter_cb = STATE_CALLBACK_NONE;
    game_state_keyboard.update_cb = keyboard_update;

    state_manager_initialize(&game_state_manager, starting_state);
}

void game_state_update()
{
    state_manager_update(&game_state_manager);
}

void game_state_combat_on_enter(state_id_t previous_id)
{
    combat_interface_start_combat();
}
