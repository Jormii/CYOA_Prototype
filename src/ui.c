#include <pspdisplay.h>

#include "ui.h"
#include "colors.h"
#include "macros.h"
#include "screen_buffer.h"

// Screens .h
#include "story_screen.h"
#include "combat_screen.h"
#include "creature_summary.h"

typedef void (*ScreenInitialization_fp)();
ScreenInitialization_fp screen_initialization_cbs[] = {
    story_screen_initialize,
    combat_screen_initialize,
    creature_summary_initialize};

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

void ui_draw_horizontal_divider(screen_t left, screen_t right, screen_t y, rgb_t color)
{
#define HORIZONTAL_DIV_MID_INCREMENT (BUFFER_WIDTH << 1)
#define HORIZONTAL_DIV_BOTTOM_INCREMENT (BUFFER_WIDTH << 2)
#define HORIZONTAL_DIV_MARGIN_WIDTH 2

    // Buffer index refers to the line drawn on the top. Middle and bottom line
    // indices are obtained through addition
    size_t buffer_index = TEXT_BUFFER_INDEX(left, y - 2);
    for (screen_t x = left; x < right; ++x)
    {
        screen_t min_distance = MIN(x - left, (right - 1) - x);
        boolean_t draw_top_and_bottom = min_distance >= HORIZONTAL_DIV_MARGIN_WIDTH;

        draw_buffer[buffer_index + HORIZONTAL_DIV_MID_INCREMENT] = color;
        if (draw_top_and_bottom)
        {
            draw_buffer[buffer_index] = color;
            draw_buffer[buffer_index + HORIZONTAL_DIV_BOTTOM_INCREMENT] = color;
        }

        buffer_index += 1;
    }
}

void ui_draw_vertical_divider(screen_t x, screen_t top, screen_t bottom, rgb_t color)
{
#define VERTICAL_DIV_MID_INCREMENT 2
#define VERTICAL_DIV_BOTTOM_INCREMENT 4
#define VERTICAL_DIV_MARGIN_WIDTH 2

    // Buffer index refers to the line drawn on the left. Middle and right line
    // indices are obtained through addition
    size_t buffer_index = TEXT_BUFFER_INDEX(x - 2, top);
    for (screen_t y = top; y < bottom; ++y)
    {
        screen_t min_distance = MIN(y - top, (bottom - 1) - y);
        boolean_t draw_left_and_right = min_distance >= VERTICAL_DIV_MARGIN_WIDTH;

        draw_buffer[buffer_index + VERTICAL_DIV_MID_INCREMENT] = color;
        if (draw_left_and_right)
        {
            draw_buffer[buffer_index] = color;
            draw_buffer[buffer_index + VERTICAL_DIV_BOTTOM_INCREMENT] = color;
        }

        buffer_index += BUFFER_WIDTH;
    }
}