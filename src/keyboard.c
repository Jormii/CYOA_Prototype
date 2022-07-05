#include "ui.h"
#include "input.h"
#include "keyboard.h"

const Key keys[] = {
    {.display = L"A", .actual_char = L'A'},
    {.display = L"B", .actual_char = L'B'}};

Keyboard keyboard = {
    .n_keys = sizeof(keys) / sizeof(Key),
    .keys = keys,

    .prompt = L"Type something...",
    .curr_buffer_length = 0,
    .curr_max_buffer_length = KEYBOARD_BUFFER_LENGTH};

void keyboard_display_buffer();
void keyboard_display_keys();
bool_t keyboard_handle_input();

State *keyboard_update()
{
    tb_clear(&(print_window.buffer), NULL);
    keyboard_display_buffer();
    keyboard_display_keys();

    bool_t exit = keyboard_handle_input();
    if (exit)
    {
        // TODO
    }

    return STATE_SAME_STATE;
}

void keyboard_display_buffer()
{
    tb_print(&(print_window.buffer), 0x00FFFFFF, keyboard.prompt);
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n\n");

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"> ");
    tb_print(&(print_window.buffer), 0x00FFFFFF, keyboard.buffer);

    for (size_t i = keyboard.curr_buffer_length; i < keyboard.curr_max_buffer_length; ++i)
    {
        tb_print(&(print_window.buffer), 0x00888888, L"_");
    }

    tb_printf(&(print_window.buffer), 0x00888888, L"\n(%u characters max)\n",
              keyboard.curr_max_buffer_length);

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n");
}

void keyboard_display_keys()
{
    for (size_t i = 0; i < keyboard.n_keys; ++i)
    {
        rgb_t color = 0x00FFFFFF;
        const Key *key = keyboard.keys + i;

        if (keyboard.cursor == i)
        {
            color = 0x0000FFFF;
        }

        tb_print(&(print_window.buffer), color, key->display);
        tb_print(&(print_window.buffer), color, L" ");
    }
}

bool_t keyboard_handle_input()
{
    bool_t exit = FALSE;
    const Key *key_under_cursor = keyboard.keys + keyboard.cursor;

    if (input_button_pressed(BUTTON_CROSS))
    {
        if (keyboard.curr_buffer_length != keyboard.curr_max_buffer_length)
        {
            keyboard.buffer[keyboard.curr_buffer_length] = key_under_cursor->actual_char;
            keyboard.curr_buffer_length += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (keyboard.curr_buffer_length != 0)
        {
            keyboard.curr_buffer_length -= 1;
            keyboard.buffer[keyboard.curr_buffer_length] = L'\0';
        }
    }
    else if (input_button_pressed(BUTTON_UP))
    {
    }
    else if (input_button_pressed(BUTTON_RIGHT))
    {
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
    }
    else if (input_button_pressed(BUTTON_RIGHT))
    {
    }

    return exit;
}