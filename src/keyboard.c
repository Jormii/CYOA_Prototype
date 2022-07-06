#include "ui.h"
#include "input.h"
#include "macros.h"
#include "keyboard.h"

#define KEYBOARD_NEW_LINE \
    {                     \
        .display = L""    \
    }

const Key keys[] = {
    {.display = L"1", .character = L'1', .allows_uppercase = FALSE},
    {.display = L"2", .character = L'2', .allows_uppercase = FALSE},
    {.display = L"3", .character = L'3', .allows_uppercase = FALSE},
    {.display = L"4", .character = L'4', .allows_uppercase = FALSE},
    {.display = L"5", .character = L'5', .allows_uppercase = FALSE},
    {.display = L"6", .character = L'6', .allows_uppercase = FALSE},
    {.display = L"7", .character = L'7', .allows_uppercase = FALSE},
    {.display = L"8", .character = L'8', .allows_uppercase = FALSE},
    {.display = L"9", .character = L'9', .allows_uppercase = FALSE},
    {.display = L"0", .character = L'0', .allows_uppercase = FALSE},
    KEYBOARD_NEW_LINE,

    {.display = L"q", .character = L'q', .allows_uppercase = TRUE},
    {.display = L"w", .character = L'w', .allows_uppercase = TRUE},
    {.display = L"e", .character = L'e', .allows_uppercase = TRUE},
    {.display = L"r", .character = L'r', .allows_uppercase = TRUE},
    {.display = L"t", .character = L't', .allows_uppercase = TRUE},
    {.display = L"y", .character = L'y', .allows_uppercase = TRUE},
    {.display = L"u", .character = L'u', .allows_uppercase = TRUE},
    {.display = L"i", .character = L'i', .allows_uppercase = TRUE},
    {.display = L"o", .character = L'o', .allows_uppercase = TRUE},
    {.display = L"p", .character = L'p', .allows_uppercase = TRUE},
    KEYBOARD_NEW_LINE,

    {.display = L"a", .character = L'a', .allows_uppercase = TRUE},
    {.display = L"s", .character = L's', .allows_uppercase = TRUE},
    {.display = L"d", .character = L'd', .allows_uppercase = TRUE},
    {.display = L"f", .character = L'f', .allows_uppercase = TRUE},
    {.display = L"g", .character = L'g', .allows_uppercase = TRUE},
    {.display = L"h", .character = L'h', .allows_uppercase = TRUE},
    {.display = L"j", .character = L'j', .allows_uppercase = TRUE},
    {.display = L"k", .character = L'k', .allows_uppercase = TRUE},
    {.display = L"l", .character = L'l', .allows_uppercase = TRUE},
    {.display = L"ñ", .character = L'ñ', .allows_uppercase = TRUE},
    KEYBOARD_NEW_LINE,

    {.display = L"z", .character = L'z', .allows_uppercase = TRUE},
    {.display = L"x", .character = L'x', .allows_uppercase = TRUE},
    {.display = L"c", .character = L'c', .allows_uppercase = TRUE},
    {.display = L"v", .character = L'v', .allows_uppercase = TRUE},
    {.display = L"b", .character = L'b', .allows_uppercase = TRUE},
    {.display = L"n", .character = L'n', .allows_uppercase = TRUE},
    {.display = L"m", .character = L'm', .allows_uppercase = TRUE}};

Keyboard keyboard = {
    .n_keys = sizeof(keys) / sizeof(Key),
    .keys = keys,
    .prompt = L"Type something..."};

wchar_t key_get_character(const Key *key);
bool_t key_is_newline(const Key *key);

void keyboard_display_buffer();
void keyboard_display_keys();
bool_t keyboard_handle_input();

size_t keyboard_up();
size_t keyboard_down();
bool_t keyboard_is_uppercase();

void keyboard_reset(size_t buffer_length)
{
    keyboard.curr_max_buffer_length = MIN(buffer_length, KEYBOARD_BUFFER_LENGTH);

    keyboard.cursor = 0;

    keyboard.uppercase = TRUE;
    keyboard.uppercase_lock = FALSE;
    keyboard.buffer[0] = L'\0';
    keyboard.buffer[keyboard.curr_max_buffer_length] = L'\0';
    keyboard.curr_buffer_length = 0;
}

State *keyboard_update()
{
    tb_clear(&(print_window.buffer), NULL);
    keyboard_display_buffer();
    keyboard_display_keys();

    bool_t quit = keyboard_handle_input();
    if (quit)
    {
        return STATE_EXIT_STATE;
    }
    else
    {
        return STATE_SAME_STATE;
    }
}

wchar_t key_get_character(const Key *key)
{
    bool_t flag = keyboard_is_uppercase() * key->allows_uppercase;
    return key->character - 32 * flag;
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
    {
        rgb_t color = (keyboard_is_uppercase()) ? 0x008888FF : 0x00FFFFFF;
        wchar_t *msg = L"No";
        if (keyboard.uppercase_lock)
        {
            msg = L"Locked";
        }
        else if (keyboard.uppercase)
        {
            msg = L"Yes";
        }
        tb_printf(&(print_window.buffer), color, L"Uppercase on? %ls\n", msg);
    }

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

    // Indicate special keys manually
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n\nX: Pulsar     ");
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"O: Borrar     ");
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"#: Espacio\n");

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"L: Minúsculas/Mayúsculas     ");
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"R: Bloquear mayúsculas\n");

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"Start: Confirmar");
}

bool_t keyboard_handle_input()
{
    bool_t quit = FALSE;
    const Key *key_under_cursor = keyboard.keys + keyboard.cursor;

    if (input_button_pressed(BUTTON_CROSS))
    {
        if (keyboard.curr_buffer_length != keyboard.curr_max_buffer_length)
        {
            keyboard.buffer[keyboard.curr_buffer_length] = key_get_character(key_under_cursor);
            keyboard.curr_buffer_length += 1;

            keyboard.uppercase = FALSE;
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (keyboard.curr_buffer_length != 0)
        {
            keyboard.curr_buffer_length -= 1;
            keyboard.buffer[keyboard.curr_buffer_length] = L'\0';

            keyboard.uppercase = keyboard.curr_buffer_length == 0;
        }
    }
    else if (input_button_pressed(BUTTON_SQUARE))
    {
        if (keyboard.curr_buffer_length != keyboard.curr_max_buffer_length)
        {
            keyboard.buffer[keyboard.curr_buffer_length] = L' ';
            keyboard.curr_buffer_length += 1;

            keyboard.uppercase = FALSE;
        }
    }
    else if (input_button_pressed(BUTTON_LT))
    {
        keyboard.uppercase = !keyboard.uppercase;
    }
    else if (input_button_pressed(BUTTON_RT))
    {
        keyboard.uppercase_lock = !keyboard.uppercase_lock;
        keyboard.uppercase = keyboard.uppercase_lock;
    }
    else if (input_button_pressed(BUTTON_LEFT))
    {
        if (keyboard.cursor != 0)
        {
            const Key *previous_key = keyboard.keys + (keyboard.cursor - 1);
            if (!key_is_newline(previous_key))
            {
                keyboard.cursor -= 1;
            }
        }
    }
    else if (input_button_pressed(BUTTON_RIGHT))
    {
        if ((keyboard.cursor + 1) != keyboard.n_keys)
        {
            const Key *next_key = keyboard.keys + (keyboard.cursor + 1);
            if (!key_is_newline(next_key))
            {
                keyboard.cursor += 1;
            }
        }
    }
    else if (input_button_pressed(BUTTON_UP))
    {
        keyboard.cursor = keyboard_up();
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
        keyboard.cursor = keyboard_down();
    }
    else if (input_button_pressed(BUTTON_START))
    {
        quit = TRUE;
    }

    return quit;
}

size_t keyboard_up()
{
    // TODO: Code duplication <=> keyboard_down()
    bool_t found_newline = FALSE;
    size_t cursor = keyboard.cursor - 1;
    for (; cursor != -1; --cursor) // TODO: Is "!= -1" hacky?
    {
        const Key *key = keyboard.keys + cursor;
        if (key_is_newline(key))
        {
            found_newline = TRUE;
        }
        else if (found_newline)
        {
            break;
        }
    }

    // Keep iterating until the next newline is found
    for (; cursor != -1; --cursor)
    {
        const Key *key = keyboard.keys + cursor;
        if (key_is_newline(key))
        {
            cursor += 1; // +1 so it doesn't point to a newline key
            break;
        }
    }

    if (cursor == -1)
    {
        // If the beginning of the layout was found, place the cursor at the
        //  start if a newline was found => Go up only if the cursor isn't
        //  located at the first line
        cursor = (found_newline) ? 0 : keyboard.cursor;
    }

    return cursor;
}

size_t keyboard_down()
{
    bool_t found_newline = FALSE;
    size_t cursor = keyboard.cursor + 1;
    for (; cursor < keyboard.n_keys; ++cursor)
    {
        const Key *key = keyboard.keys + cursor;
        if (key_is_newline(key))
        {
            found_newline = TRUE;
        }
        else if (found_newline)
        {
            break;
        }
    }

    if (cursor == keyboard.n_keys)
    {
        // If the end of the layout was found, do not modify cursor
        cursor = keyboard.cursor;
    }

    return cursor;
}

bool_t keyboard_is_uppercase()
{
    return keyboard.uppercase || keyboard.uppercase_lock;
}
