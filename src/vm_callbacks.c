#include <stdlib.h>

#include <pspiofilemgr.h>

#include "ui.h"
#include "input.h"
#include "virtual_machine.h"

void call_cb(uint32_t hash)
{
    // TODO
}

void print_cb(const wchar_t *string)
{
    tb_printf(&(print_window.buffer), RGB(255, 255, 255), L"%ls\n", string);
}

uint16_t chosen = 0;
void print_option_cb(uint16_t index, const wchar_t *string)
{
    if (input_button_pressed(BUTTON_UP) && chosen > 0)
    {
        chosen -= 1;
    }
    else if (input_button_pressed(BUTTON_DOWN) && (chosen + 1) < vm.header.options_count)
    {
        chosen += 1;
    }

    rgb_t color = 0xFFFFFFFF;
    if (chosen == index)
    {
        color = RGB(255, 255, 0);
    }
    tb_printf(&(commands_window.buffer), color, L"%u: %ls\n", index, string);
}

void *read_file_cb(const char *path, size_t *out_size)
{
    SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
    if (fd < 0)
    {
        return 0;
    }

    SceIoStat stats;
    sceIoGetstat(path, &stats);

    *out_size = stats.st_size;
    void *buffer = malloc(*out_size);
    sceIoRead(fd, buffer, *out_size);
    sceIoClose(fd);

    return buffer;
}

void save_program_cb(const char *path)
{
    size_t offset = sizeof(vm_header_t) + vm.header.options_count * sizeof(vm_option_t);
    uint8_t *ints_ptr = vm.program_bytes + offset;

    SceUID fd = sceIoOpen(path, PSP_O_WRONLY, 0777);
    sceIoLseek(fd, offset, PSP_SEEK_CUR);
    sceIoWrite(fd, ints_ptr, sizeof(vm_int_t) * vm.header.integers_count);
    sceIoClose(fd);
}

void save_global_vars_cb(const char *path)
{
    SceUID fd = sceIoOpen(path, PSP_O_WRONLY, 0777);
    sceIoWrite(fd, vm.global_variables, sizeof(vm_int_t) * vm.global_variables_count);
    sceIoClose(fd);
}