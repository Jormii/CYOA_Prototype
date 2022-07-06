#include "state.h"

void state_manager_initialize(StateManager *manager, State *starting_state)
{
    manager->running = TRUE;
    manager->state = starting_state;
    if (starting_state->on_enter_cb != STATE_CALLBACK_NONE)
    {
        starting_state->on_enter_cb(-1); // TODO: Weird
    }
}

void state_manager_spawn(State *starting_state)
{
    StateManager manager;
    state_manager_initialize(&manager, starting_state);
    while (manager.running)
    {
        state_manager_update(&manager);
    }
}

void state_manager_update(StateManager *manager)
{
    State *current = manager->state;
    State *returned = current->update_cb();
    if (returned != STATE_SAME_STATE && returned->id != current->id)
    {
        if (returned->on_enter_cb != STATE_CALLBACK_NONE)
        {
            returned->on_enter_cb(current->id);
        }

        manager->state = returned;
    }
}