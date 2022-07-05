#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "state.h"
#include "types.h"

#define KEYBOARD_BUFFER_LENGTH 16

typedef struct Key_st
{
    wchar_t character;
    const wchar_t *display;
    bool_t allows_uppercase;
} Key;

typedef struct Keyboard_st
{
    bool_t running;

    size_t n_keys;
    const Key *keys;
    size_t cursor;

    bool_t uppercase;
    bool_t uppercase_lock;
    wchar_t *prompt;
    wchar_t buffer[KEYBOARD_BUFFER_LENGTH + 1];
    size_t curr_buffer_length;
    size_t curr_max_buffer_length;
} Keyboard;

extern Keyboard keyboard;

void keyboard_reset(size_t buffer_length);
void keyboard_update();

#endif