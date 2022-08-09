#include <stdio.h>
#include <stdlib.h>

#include <pspiofilemgr.h>

#include "input.h"
#include "cyoa_interface.h"
#include "special_characters.h"

#define MOVE_UP -1
#define MOVE_DOWN 1

void cyoa_input_update();
void cyoa_options_move(int direction, int32_t limit);

void cyoa_interface_print(const wchar_t *string, uint8_t is_option);
void cyoa_interface_end_of_string(uint8_t is_option);
void cyoa_interface_option_start(uint16_t index);
void *cyoa_interface_read(const char *path, size_t *out_size);
void cyoa_interface_save(const char *path);
void cyoa_interface_save_global_vars(const char *path);
extern void cyoa_interface_call(uint32_t hash);

void cyoa_interface_initialize(uint32_t starting_scene, uint16_t max_options, uint8_t max_stack_size,
                               TextBuffer *messages_buffer, TextBuffer *choices_buffer)
{
    if (sizeof(size_t) > sizeof(uint32_t))
    {
        fprintf(stderr,
                "Pointer size should be at most 4 bytes. sizeof(size_t) <- %u",
                sizeof(size_t));
        return;
    }

    cyoa_interface.color = 0x00FFFFFF;
    cyoa_interface.previous_color = cyoa_interface.color;
    cyoa_interface.option_cursor = 0;
    cyoa_interface.execute_option = FALSE;
    cyoa_interface.scene_switched = TRUE;
    cyoa_interface.messages_buffer = messages_buffer;
    cyoa_interface.choices_buffer = choices_buffer;

    VMInitialization vm_manager_init_values = {
        .max_options = max_options,
        .max_stack_size = max_stack_size,

#ifndef PPSSPP
        .binaries_dir = "ms0:/PSP/GAME/compilation/assets/binaries/",
        .global_vars_path = "ms0:/PSP/GAME/compilation/assets/binaries/global.bin",
#else
        .binaries_dir = "umd0:/assets/binaries/",
        .global_vars_path = "umd0:/assets/binaries/global.bin",

#endif

        .call_cb = cyoa_interface_call,
        .print_cb = cyoa_interface_print,
        .end_of_string_cb = cyoa_interface_end_of_string,
        .option_start_cb = cyoa_interface_option_start,
        .read_cb = cyoa_interface_read,
        .save_program_cb = cyoa_interface_save,
        .save_global_vars_cb = cyoa_interface_save_global_vars};

    if (!vm_manager_initialize(&vm_manager_init_values))
    {
        fprintf(stderr, "Couldn't initialize VirtualMachine manager\n");
        return;
    }

    if (!vm_manager_load_program(starting_scene))
    {
        fprintf(stderr, "Couldn't load starting scene %s\n", vm_manager_curr_program());
        return;
    }
}

State *cyoa_interface_update()
{
    cyoa_interface.execute_option = FALSE;
    cyoa_input_update();

    if (cyoa_interface.scene_switched)
    {
        tb_clear(cyoa_interface.messages_buffer, NULL);
        cyoa_interface.scene_switched = vm_execute();
    }

    tb_clear(cyoa_interface.choices_buffer, NULL);
    if (!cyoa_interface.scene_switched)
    {
        vm_display_options();

        if (cyoa_interface.execute_option)
        {
            tb_clear(cyoa_interface.messages_buffer, NULL);
            cyoa_interface.scene_switched = vm_execute_option(cyoa_interface.option_cursor);
            tb_print(cyoa_interface.messages_buffer, 0x00FFFFFF, L"--------------------\n\n");

            if (!cyoa_interface.scene_switched)
            {
                cyoa_interface.scene_switched = vm_execute();
            }
        }
    }

    return STATE_SAME_STATE;
}

void cyoa_input_update()
{
    if (input_button_pressed(BUTTON_UP))
    {
        cyoa_options_move(MOVE_UP, -1);
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
        cyoa_options_move(MOVE_DOWN, vm.header.options_count);
    }

    if (input_button_pressed(BUTTON_CROSS))
    {
        cyoa_interface.execute_option = TRUE;
    }
}

void cyoa_options_move(int direction, int32_t limit)
{
    int32_t new_cursor = cyoa_interface.option_cursor + direction;
    while (new_cursor != limit && !vm.visible_options[new_cursor])
    {
        new_cursor += direction;
    }

    if (new_cursor != limit)
    {
        cyoa_interface.option_cursor = (uint16_t)new_cursor;
    }
}

void cyoa_interface_print(const wchar_t *string, uint8_t is_option)
{
    TextBuffer *dst = (is_option) ? cyoa_interface.choices_buffer : cyoa_interface.messages_buffer;
    tb_print(dst, cyoa_interface.color, string);

    if (is_option && cyoa_interface.cursor_is_over_option)
    {
        tb_printf(dst, 0x0000FFFF, L" (%lc)", SPECIAL_CHARACTER_CROSS);
    }
}

void cyoa_interface_end_of_string(uint8_t is_option)
{
    if (is_option)
    {
        tb_print(cyoa_interface.choices_buffer, cyoa_interface.color, L"\n");
    }
    else
    {
        tb_print(cyoa_interface.messages_buffer, cyoa_interface.color, L"\n\n");
    }

    cyoa_interface.color = 0x00FFFFFF;
    cyoa_interface.previous_color = cyoa_interface.color;
}

void cyoa_interface_option_start(uint16_t index)
{
    cyoa_interface.cursor_is_over_option = index == cyoa_interface.option_cursor;
    cyoa_interface.color = (cyoa_interface.cursor_is_over_option) ? 0x0000FFFF : 0x00FFFFFF;
}

void *cyoa_interface_read(const char *path, size_t *out_size)
{
    SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
    if (fd < 0)
    {
        return NULL;
    }

    SceIoStat stats;
    sceIoGetstat(path, &stats);

    *out_size = stats.st_size;
    void *buffer = malloc(*out_size);
    sceIoRead(fd, buffer, *out_size);
    sceIoClose(fd);

    return buffer;
}

void cyoa_interface_save(const char *path)
{
    size_t offset = sizeof(vm_header_t) + vm.header.options_count * sizeof(vm_option_t);
    const uint8_t *ints_ptr = vm.program_bytes + offset;

    SceUID fd = sceIoOpen(path, PSP_O_WRONLY, 0777);
    sceIoLseek(fd, offset, PSP_SEEK_CUR);
    sceIoWrite(fd, ints_ptr, vm.header.store_integers_count * sizeof(vm_int_t));
    sceIoClose(fd);
}

void cyoa_interface_save_global_vars(const char *path)
{
    SceUID fd = sceIoOpen(path, PSP_O_WRONLY, 0777);
    sceIoWrite(fd, vm.global_variables, vm.global_variables_count * sizeof(vm_int_t));
    sceIoClose(fd);
}