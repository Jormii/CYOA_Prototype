#ifndef SCREEN_COMMON_H
#define SCREEN_COMMON_H

#include "screen_buffer.h"

#define SCREEN_COMMON_HOR_DIV_Y (0.66 * SCREEN_HEIGHT)
#define SCREEN_COMMON_VER_DIV_X (0.25 * SCREEN_WIDTH)
#define SCREEN_COMMON_MARGIN 5
#define SCREEN_EXTRA_MARGIN (SCREEN_COMMON_MARGIN + 2 + 2)

void screen_common_update();

#endif