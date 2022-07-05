#include "ui.h"
#include "input.h"
#include "keyboard.h"

#define KEYBOARD_NEW_LINE                    \
    {                                        \
        .display = L"", .actual_char = L'\0' \
    }

const Key keys[] = {
    {.display = L"1", .actual_char = L'1'},
    {.display = L"2", .actual_char = L'2'},
    {.display = L"3", .actual_char = L'3'},
    {.display = L"4", .actual_char = L'4'},
    {.display = L"5", .actual_char = L'5'},
    {.display = L"6", .actual_char = L'6'},
    {.display = L"7", .actual_char = L'7'},
    {.display = L"8", .actual_char = L'8'},
    {.display = L"9", .actual_char = L'9'},
    {.display = L"0", .actual_char = L'0'},
    KEYBOARD_NEW_LINE,

    {.display = L"q", .actual_char = L'q'},
    {.display = L"w", .actual_char = L'w'},
    {.display = L"e", .actual_char = L'e'},
    {.display = L"r", .actual_char = L'r'},
    {.display = L"t", .actual_char = L't'},
    {.display = L"y", .actual_char = L'y'},
    {.display = L"u", .actual_char = L'u'},
    {.display = L"i", .actual_char = L'i'},
    {.display = L"o", .actual_char = L'o'},
    {.display = L"p", .actual_char = L'p'},
    KEYBOARD_NEW_LINE,

    {.display = L"a", .actual_char = L'a'},
    {.display = L"s", .actual_char = L's'},
    {.display = L"d", .actual_char = L'd'},
    {.display = L"f", .actual_char = L'f'},
    {.display = L"g", .actual_char = L'g'},
    {.display = L"h", .actual_char = L'h'},
    {.display = L"j", .actual_char = L'j'},
    {.display = L"k", .actual_char = L'k'},
    {.display = L"l", .actual_char = L'l'},
    {.display = L"ñ", .actual_char = L'ñ'},
    KEYBOARD_NEW_LINE,

    {.display = L"z", .actual_char = L'z'},
    {.display = L"x", .actual_char = L'x'},
    {.display = L"c", .actual_char = L'c'},
    {.display = L"v", .actual_char = L'v'},
    {.display = L"b", .actual_char = L'b'},
    {.display = L"n", .actual_char = L'n'},
    {.display = L"m", .actual_char = L'm'},
    KEYBOARD_NEW_LINE,
    KEYBOARD_NEW_LINE,

    {.display = L"[Espacio]", .actual_char = L'\0'},
    {.display = L"[Borrar]", .actual_char = L'\0'},
    {.display = L"[Confirmar]", .actual_char = L'\0'}};

Keyboard keyboard = {
    .n_keys = sizeof(keys) / sizeof(Key),
    .keys = keys,

    .prompt = L"Type something...",
    .curr_buffer_length = 0,
    .curr_max_buffer_length = KEYBOARD_BUFFER_LENGTH};

bool_t key_is_newline(const Key *key);

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

bool_t key_is_newline(const Key *key)
{
    return key->display[0] == L'\0';
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
        const Key *key = keyboard.keys + i;
        if (key_is_newline(key))
        {
            tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n");
            continue;
        }

        rgb_t color = 0x00FFFFFF;
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