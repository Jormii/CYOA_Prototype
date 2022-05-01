#include <pspkernel.h>
#include <pspdisplay.h>

#include "callbacks.h"
#include "screen_buffer.h"

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
    setup_callbacks();

    sb_initialize();
}

void game_loop()
{
    sb_swap_buffers();
    sceDisplayWaitVblankStart();
}

void deinitialize()
{
    sceKernelExitGame();
}