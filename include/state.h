#ifndef STATE_H
#define STATE_H

#include "types.h"

#define STATE_SAME_STATE NULL
#define STATE_CALLBACK_NONE NULL

struct State_st;

typedef size_t state_id_t;
typedef struct State_st *(*StateUpdate_fp)();
typedef void (*StateOnEnter_fp)(state_id_t previous_id);

typedef struct State_st
{
    state_id_t id;
    StateOnEnter_fp on_enter_cb;
    StateUpdate_fp update_cb;
} State;

typedef struct StateManager_st
{
    bool_t running;
    State *state;
} StateManager;

void state_manager_initialize(StateManager *manager, State *starting_state);
void state_manager_spawn(State *starting_state);
void state_manager_update(StateManager *manager);

#endif