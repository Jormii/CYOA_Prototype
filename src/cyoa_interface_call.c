#include <stdio.h>
#include <stdlib.h>

#include "vm_stack.h"
#include "virtual_machine.h"

extern uint16_t *open_keyboard(int32_t a0);

void cyoa_interface_call(uint32_t hash)
{
    switch (hash)
    {
    case 15266:
    {
        int32_t a0 = vm_stack_pop(&(vm.stack));
        vm_stack_push(&(vm.stack), (vm_stack_t)open_keyboard(a0));
    }
    break;
    default:
        printf("Unknown function hash %u\n", hash);
        exit(1);
    }
}
