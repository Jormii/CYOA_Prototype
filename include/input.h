#ifndef INPUT_H
#define INPUT_H

#include <pspctrl.h>

#include "types.h"

typedef enum InputButton_en
{
    BUTTON_UP = PSP_CTRL_UP,
    BUTTON_RIGHT = PSP_CTRL_RIGHT,
    BUTTON_DOWN = PSP_CTRL_DOWN,
    BUTTON_LEFT = PSP_CTRL_LEFT,

    BUTTON_TRIANGLE = PSP_CTRL_TRIANGLE,
    BUTTON_CIRCLE = PSP_CTRL_CIRCLE,
    BUTTON_CROSS = PSP_CTRL_CROSS,
    BUTTON_SQUARE = PSP_CTRL_SQUARE,

    BUTTON_LT = PSP_CTRL_LTRIGGER,
    BUTTON_RT = PSP_CTRL_RTRIGGER,

    BUTTON_START = PSP_CTRL_START
} InputButton;

void input_init();
void input_update();
bool_t input_button_pressed(InputButton button);
bool_t input_button_held(InputButton button);

#endif