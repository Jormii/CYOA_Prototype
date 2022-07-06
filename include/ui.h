#ifndef UI_H
#define UI_H

#include "types.h"
#include "window.h"
#include "scrollbar.h"

Window print_window;
Window commands_window;
Window keyboard_window;

bool_t print_window_visible;
bool_t commands_window_visible;
bool_t keyboard_window_visible;

void ui_initialize();
void ui_update();

void ui_hide_all();

#endif