#include <stdio.h>

#include "ui.h"
#include "input.h"
#include "keyboard.h"
#include "game_state.h"
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

uint16_t *open_keyboard(int32_t buffer_length)
{
    keyboard_reset(buffer_length);
    while (keyboard.running)
    {
        input_update();
        keyboard_update();
        ui_update(); // TODO: Ugly
    }

    return (uint16_t *)keyboard.buffer;
}