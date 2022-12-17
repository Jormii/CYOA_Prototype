
#include <stdio.h>
#include <stdlib.h>

#include "vm_stack.h"
#include "virtual_machine.h"



void vm_call_function(uint32_t hash) {
    switch (hash) {
    
    default:
        printf("Unknown function hash %u\n", hash);
        exit(1);
    }
}
