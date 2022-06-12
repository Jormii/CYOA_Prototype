#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef void (*GameStateCb_fp)();

typedef struct GameState_st
{
    GameStateCb_fp update_cb;
    GameStateCb_fp on_enter_cb;
} GameState;

typedef enum GameStateIdentifier_en
{
    GAME_STATE_NONE,
    GAME_STATE_STORY,
    GAME_STATE_COMBAT,
    GAME_STATE_COUNT
} GameStateIdentifier;

void game_state_initialize();
void game_state_update();

void game_state_switch_to(GameStateIdentifier state_id);

#endif