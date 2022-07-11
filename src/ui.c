#include <pspdisplay.h>

#include "ui.h"
#include "colors.h"
#include "screen_buffer.h"

// Screens .h
#include "title_screen.h"

// TODO: Remove
Window print_window;
Window commands_window;
Window keyboard_window;

typedef void (*ScreenInitialization_fp)();
ScreenInitialization_fp screen_initialization_cbs[] = {
    title_screen_initialize};

void ui_initialize()
{
    sb_initialize();

    size_t n_screens = sizeof(screen_initialization_cbs) / sizeof(ScreenInitialization_fp *);
    for (size_t i = 0; i < n_screens; ++i)
    {
        screen_initialization_cbs[i]();
    }
}

void ui_update()
{
    sb_swap_buffers();
    sb_clear_buffer(COLOR_BLACK);
    sceDisplayWaitVblankStart();
}