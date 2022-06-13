#include <pspkernel.h>
#include <pspdisplay.h>

#include "ui.h"
#include "log.h"
#include "input.h"
#include "callbacks.h"
#include "game_state.h"
#include "screen_buffer.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

PSP_MODULE_INFO("Prototype", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

void initialize();
void game_loop();
void deinitialize();

int main()
{
    initialize();
    while (running())
    {
        game_loop();
    }
    deinitialize();

    return 0;
}

void initialize()
{
    log_fd = -1; // TODO: Somewhere else

    // System and interface
    setup_callbacks();

    input_init();
    sb_initialize(); // TODO: Should be part of the UI
    ui_initialize();

    // Initialize CYOA engine
    uint16_t max_options = 3;
    uint8_t max_stack_size = 1;
    cyoa_interface_initialize(0, max_options, max_stack_size);

    // Initialize combat engine
    combat_interface_initialize();

    // Initialize game state
    game_state_initialize(&game_state_combat);
}

void game_loop()
{
    input_update();
    game_state_update();

    ui_update();
    sb_swap_buffers();
    sceDisplayWaitVblankStart();
}

void deinitialize()
{
    LOG_CLOSE();
    sceKernelExitGame();
}