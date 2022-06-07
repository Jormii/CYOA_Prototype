#include "game_state.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

typedef void (*GameStateFunc_fp)();

typedef enum GameState_en
{
    STORY,
    COMBAT,
    GAME_STATE_COUNT
} GameState;

GameState current_state;
GameStateFunc_fp state_cbs[GAME_STATE_COUNT];

void game_state_initialize()
{
    current_state = COMBAT;
    state_cbs[STORY] = cyoa_interface_update;
    state_cbs[COMBAT] = combat_interface_update;
}

void game_state_update()
{
    state_cbs[current_state]();
}