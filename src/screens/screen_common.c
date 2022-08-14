#include "ui.h"
#include "screen_common.h"

void screen_common_update()
{
    ui_draw_horizontal_divider(
        SCREEN_COMMON_VER_DIV_X + SCREEN_COMMON_MARGIN, SCREEN_WIDTH - 1,
        SCREEN_COMMON_HOR_DIV_Y,
        0x00888888);
    ui_draw_vertical_divider(
        SCREEN_COMMON_VER_DIV_X,
        1, SCREEN_HEIGHT - 1,
        0x00888888);
}