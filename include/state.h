#ifndef STATE_H
#define STATE_H

#include "types.h"

#define STATE_SAME_STATE NULL
#define STATE_CALLBACK_NONE NULL

struct State_st;

typedef size_t state_id_t;
typedef struct State_st *(*StateFunc_fp)();
typedef void (*StateOnEnter_fp)(state_id_t previous_id);
typedef void (*StateOnExit_fp)(state_id_t next_id);

typedef struct State_st
{
    state_id_t id;
    StateFunc_fp func_cb;
    StateOnEnter_fp on_enter_cb;
    StateOnExit_fp on_exit_cb;
} State;

typedef struct StateManager_st
{
    State *state;
} StateManager;

void state_manager_initialize(StateManager *manager, State *starting_state);
void state_manager_execute(StateManager *manager);

#endif