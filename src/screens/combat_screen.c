#include "base_set_font.h"
#include "combat_screen.h"
#include "screen_buffer.h"
#include "screen_common.h"
#include "window_display.h"
#include "combat_interface.h"
#include "creature_summary.h"

#define COMBAT_STATE_BUFFER_LENGTH 1024
#define COMBAT_LOG_BUFFER_LENGTH 1024
#define COMBAT_COMMANDS_BUFFER_LENGTH 1024

Window combat_state_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_MARGIN,
        .bottom = SCREEN_COMMON_HOR_DIV_Y - SCREEN_EXTRA_MARGIN},
    .font = &base_set_font};

Window combat_log_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_MARGIN,
        .bottom = SCREEN_COMMON_HOR_DIV_Y - SCREEN_EXTRA_MARGIN},
    .font = &base_set_font};

Window combat_commands_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_HOR_DIV_Y + SCREEN_EXTRA_MARGIN,
        .bottom = SCREEN_HEIGHT - SCREEN_COMMON_MARGIN},
    .font = &base_set_font};

void combat_screen_initialize()
{
    create_text_buffer(COMBAT_STATE_BUFFER_LENGTH, &(combat_state_window.buffer));
    create_text_buffer(COMBAT_LOG_BUFFER_LENGTH, &(combat_log_window.buffer));
    create_text_buffer(COMBAT_COMMANDS_BUFFER_LENGTH, &(combat_commands_window.buffer));
}

void combat_screen_update()
{
    if (combat_interface.displaying_state)
    {
        display_window(&combat_state_window);
    }
    else
    {
        display_window(&combat_log_window);
    }
    display_window(&combat_commands_window);
    creature_summary_update();
    screen_common_update();
}