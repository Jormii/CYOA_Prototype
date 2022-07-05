#include "ui.h"
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
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"_\n");
    tb_printf(&(print_window.buffer), 0x00888888, L"(%u characters max)\n",
              keyboard.curr_max_buffer_length);

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n");
}

void keyboard_display_keys()
{
    for (size_t i = 0; i < keyboard.n_keys; ++i)
    {
        const Key *key = keyboard.keys + i;
        tb_print(&(print_window.buffer), 0x00FFFFFF, key->display);
        tb_print(&(print_window.buffer), 0x00FFFFFF, L" ");
    }
}

bool_t keyboard_handle_input()
{
    return FALSE;
}