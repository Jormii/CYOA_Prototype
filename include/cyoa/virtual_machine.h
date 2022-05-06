#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "fields.h"
#include "vm_stack.h"

struct
{
    // Program exclusive data
    Header header;
    uint8_t *program_bytes;

    // Global data
    VMStack stack;
    uint8_t *visible_options;
    vm_int_t *global_variables;
    size_t global_variables_count;

    // Execution data
    uint8_t executing;
    uint32_t pc;
    Instruction inst; // Instruction being executed at a given time
    int32_t v1;       // v1, v2: Left and right operands. Or single operand
    int32_t v2;
} vm;

uint8_t vm_load_program(const char *program_path);
void vm_execute();
void vm_display_options();
extern void vm_print_option(uint16_t index, const wchar_t *string);

#endif