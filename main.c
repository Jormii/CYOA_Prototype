#include <pspkernel.h>
#include <pspdisplay.h>

#include "ui.h"
#include "types.h"
#include "callbacks.h"
#include "text_buffer.h"
#include "screen_buffer.h"

PSP_MODULE_INFO("Prototype", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

bool_t found_error;

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
    found_error = FALSE;

    setup_callbacks();

    sb_initialize();
    ui_initialize();

    // TODO: This is cyoa's engine's business. Move to its right place later
    if (sizeof(size_t) != sizeof(uint32_t))
    {
        tb_clear(&(print_window.buffer), NULL);
        tb_printf(&(print_window.buffer), RGB(255, 122, 122),
                  L"Pointer size should be 4 bytes. sizeof(size_t) <- %lu", sizeof(size_t));
        found_error = TRUE;
    }
}

void game_loop()
{
    ui_update();
    sb_swap_buffers();
    sceDisplayWaitVblankStart();
}

void deinitialize()
{
    sceKernelExitGame();
}