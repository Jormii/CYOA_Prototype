#ifndef COMBAT_SCREEN_H
#define COMBAT_SCREEN_H

#include "window.h"

extern Window combat_state_window;
extern Window combat_log_window;
extern Window combat_commands_window;

void combat_screen_initialize();
void combat_screen_update();

#endif