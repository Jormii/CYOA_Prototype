#ifndef UI_H
#define UI_H

#include "window.h"
#include "scrollbar.h"

Window print_window;
Scrollbar print_scrollbar;
Window commands_window;

void ui_initialize();
void ui_update();

#endif