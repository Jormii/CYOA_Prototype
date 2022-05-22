#ifndef INPUT_H
#define INPUT_H

#include <pspctrl.h>

#include "types.h"

typedef enum InputButton_en
{
    BUTTON_UP = PSP_CTRL_UP,
    BUTTON_DOWN = PSP_CTRL_DOWN,
    BUTTON_CROSS = PSP_CTRL_CROSS,
    BUTTON_CIRCLE = PSP_CTRL_CIRCLE,
    BUTTON_LT = PSP_CTRL_LTRIGGER,
    BUTTON_RT = PSP_CTRL_RTRIGGER
} InputButton;

void input_init();
void input_update();
bool_t input_button_pressed(InputButton button);

#endif