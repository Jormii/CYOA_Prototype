#ifndef DIE_H
#define DIE_H

#include "types.h"

typedef struct Die_st
{
    u8_t amount;
    u8_t sides;
} Die;

const Die *get_die(stat_t stat);
u8_t roll_die(const Die *die);

#endif