#ifndef UI_H
#define UI_H

#include "window.h"

#define WIDE_STR(s) L##s
#define UI_WINDOW_PRINT(w, col, s) tb_print(&(w.buffer), col, WIDE_STR(s))
#define UI_WINDOW_PRINTF(w, col, fmt, ...) tb_printf(&(w.buffer), col, WIDE_STR(fmt), ##__VA_ARGS__)

// TODO: Remove
extern Window print_window;
extern Window commands_window;
extern Window keyboard_window;

void ui_initialize();
void ui_update();

#endif