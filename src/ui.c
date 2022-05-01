#include "ui.h"
#include "base_set_font.h"
#include "screen_buffer.h"
#include "margin_display.h" // TODO: Remove
#include "window_display.h"
#include "scrollbar_display.h"

void init_window(Window *w, size_t buffer_length);

void ui_initialize()
{
    init_window(&print_window, 2048);
    init_window(&commands_window, 1024);

    screen_t divider = 0.75f * SCREEN_HEIGHT;

    print_window.margin.left = 1;
    print_window.margin.right = SCREEN_WIDTH - 10;
    print_window.margin.top = 1;
    print_window.margin.bottom = divider - 1;

    commands_window.margin.left = 1;
    commands_window.margin.right = SCREEN_WIDTH - 2;
    commands_window.margin.top = divider + 1;
    commands_window.margin.bottom = SCREEN_HEIGHT - 2;

    print_scrollbar.window = &print_window;
    print_scrollbar.margin_left = print_window.margin.right + 2;
    print_scrollbar.margin_right = SCREEN_WIDTH - 2;
}

void ui_update()
{
    display_window(&print_window);
    display_window(&commands_window);
    display_scrollbar(&print_scrollbar, RGB(255, 255, 255));

    // TODO: Remove these
    display_margin(&(print_window.margin), RGB(50, 50, 50));
    display_margin(&(commands_window.margin), RGB(50, 50, 50));
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