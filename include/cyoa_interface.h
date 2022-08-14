#ifndef CYOA_INTERFACE_H
#define CYOA_INTERFACE_H

#include "state.h"
#include "types.h"
#include "pspp_types.h"
#include "vm_manager.h"
#include "text_buffer.h"
#include "virtual_machine.h"

struct
{
    rgb_t color;
    rgb_t previous_color;
    uint16_t option_cursor;
    bool_t cursor_is_over_option;
    bool_t execute_option;
    bool_t scene_switched;
    TextBuffer *messages_buffer;
    TextBuffer *choices_buffer;
} cyoa_interface;

void cyoa_interface_initialize(uint32_t starting_scene, uint16_t max_options, uint8_t max_stack_size,
                               TextBuffer *messages_buffer, TextBuffer *choices_buffer);
State *cyoa_interface_update();

#endif