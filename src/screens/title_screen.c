#include "ui.h"
#include "colors.h"
#include "title_screen.h"
#include "base_set_font.h"
#include "screen_buffer.h"
#include "window_display.h"

Window title_name = {
    .margin = {
        .left = 0,
        .right = SCREEN_WIDTH - 1,
        .top = 0,
        .bottom = 20 - 1},
    .font = &base_set_font};

Window title_new = {
    .margin = {
        .left = 0,
        .right = SCREEN_WIDTH - 1,
        .top = 20,
        .bottom = 40 - 1},
    .font = &base_set_font};

Window title_continue = {
    .margin = {
        .left = 0,
        .right = SCREEN_WIDTH - 1,
        .top = 40,
        .bottom = 60 - 1},
    .font = &base_set_font};

void title_screen_initialize()
{
    create_text_buffer(100, &(title_name.buffer));
    create_text_buffer(100, &(title_new.buffer));
    create_text_buffer(100, &(title_continue.buffer));

    UI_WINDOW_PRINT(title_name, COLOR_WHITE, "-- GAME TITLE --");
    UI_WINDOW_PRINT(title_new, COLOR_WHITE, "New game");
    UI_WINDOW_PRINT(title_continue, COLOR_WHITE, "Continue playing");
}

void title_screen_update()
{
    display_window(&title_name);
    display_window(&title_new);
    display_window(&title_continue);
}