#include "input.h"

SceCtrlLatch latch;

void input_init()
{
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
}

void input_update()
{
    sceCtrlReadLatch(&latch);
}

bool_t input_button_pressed(InputButton button)
{
    return (latch.uiMake & button) != 0;
}