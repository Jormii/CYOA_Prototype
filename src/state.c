#include "state.h"

void state_manager_execute(StateManager *manager)
{
    State *current = manager->state;
    State *returned = current->func_cb();
    if (returned != STATE_SAME_STATE && returned->id != current->id)
    {
        if (current->on_exit_cb != STATE_CALLBACK_NONE)
        {
            current->on_exit_cb(returned->id);
        }
        if (returned->on_enter_cb != STATE_CALLBACK_NONE)
        {
            returned->on_enter_cb(current->id);
        }

        manager->state = returned;
    }
}