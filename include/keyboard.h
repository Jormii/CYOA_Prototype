#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "state.h"
#include "types.h"

#define KEYBOARD_BUFFER_LENGTH 16

typedef struct Key_st
{
    wchar_t *display;
    wchar_t actual_char;
} Key;

typedef struct Keyboard_st
{
    size_t n_keys;
    const Key *keys;
    size_t cursor;

    wchar_t *prompt;
    wchar_t buffer[KEYBOARD_BUFFER_LENGTH + 1];
    size_t curr_buffer_length;
    size_t curr_max_buffer_length;
} Keyboard;

extern Keyboard keyboard;

State *keyboard_update();

#endif