#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state.h"

extern State game_state_story;
extern State game_state_combat;
extern State game_state_keyboard;

void game_state_initialize(State *starting_state);
void game_state_update();

#endif