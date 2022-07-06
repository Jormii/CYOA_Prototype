#include <pspdisplay.h>

#include "ui.h"
#include "input.h"
#include "callbacks.h"
#include "game_loop.h"
#include "screen_buffer.h"

void game_loop_spawn(State *starting_state)
{
    StateManager manager;
    state_manager_initialize(&manager, starting_state);

    while (running() && manager.running)
    {
        input_update();
        state_manager_update(&manager);

        ui_update();
        sb_swap_buffers();
        sceDisplayWaitVblankStart();
    }
}