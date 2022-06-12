#include <stdio.h>

#include "cyoa_interface.h"

uint16_t *color(int32_t r, int32_t g, int32_t b)
{
    cyoa_interface.previous_color = cyoa_interface.color;
    cyoa_interface.color = RGB(r, g, b);
    return NULL;
}

uint16_t *previous_color()
{
    cyoa_interface.color = cyoa_interface.previous_color;
    return NULL;
}