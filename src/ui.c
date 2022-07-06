#include "ui.h"
#include "base_set_font.h"
#include "screen_buffer.h"
#include "margin_display.h" // TODO: Remove
#include "window_display.h"
#include "scrollbar_display.h"

void init_window(Window *w, size_t buffer_length);

void ui_initialize()
{
    ui_hide_all();

    init_window(&print_window, 2048);
    init_window(&commands_window, 1024);
    init_window(&keyboard_window, 256);

    screen_t divider = 0.66f * SCREEN_HEIGHT;

    print_window.margin.left = 1;
    print_window.margin.right = SCREEN_WIDTH - 10;
    print_window.margin.top = 1;
    print_window.margin.bottom = divider - 1;

    commands_window.margin.left = 1;
    commands_window.margin.right = SCREEN_WIDTH - 2;
    commands_window.margin.top = divider + 1;
    commands_window.margin.bottom = SCREEN_HEIGHT - 2;

    keyboard_window.margin = print_window.margin;
}

void ui_update()
{
    if (print_window_visible)
    {
        display_window(&print_window);
    }

    if (commands_window_visible)
    {
        display_window(&commands_window);
    }

    if (keyboard_window_visible)
    {
        display_window(&keyboard_window);
    }
}

void ui_hide_all()
{
    print_window_visible = FALSE;
    commands_window_visible = FALSE;
    keyboard_window_visible = FALSE;
}

void init_window(Window *w, size_t buffer_length)
{
    // Margin is initialized outside
    create_text_buffer(buffer_length, &(w->buffer));
    w->buffer.overflow_cb = w_buffer_overflow_cb;
    w->buffer.ptr_cb = w;
    w->font = &base_set_font;
    w->scroll_amount = 0;
}