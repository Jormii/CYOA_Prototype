#ifndef VM_CALLBACKS_H
#define VM_CALLBACKS_H

#include <stddef.h>
#include <stdint.h>

void call_cb(uint32_t hash);
void print_cb(const wchar_t *string);
void print_option_cb(uint16_t index, const wchar_t *string);
void *read_file_cb(const char *path, size_t *out_size);
void save_program_cb(const char *path);
void save_global_vars_cb(const char *path);

#endif