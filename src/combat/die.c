#include <stdlib.h>

#include "die.h"
#include "macros.h"

#define N_DICE 18

Die dice[N_DICE] = {
    {.amount = 2, .sides = 4},
    {.amount = 2, .sides = 6},
    {.amount = 3, .sides = 4},
    {.amount = 2, .sides = 8},
    {.amount = 2, .sides = 10},
    {.amount = 3, .sides = 6},
    {.amount = 4, .sides = 4},
    {.amount = 2, .sides = 12},
    {.amount = 3, .sides = 8},
    {.amount = 4, .sides = 6},
    {.amount = 3, .sides = 10},
    {.amount = 2, .sides = 20},
    {.amount = 4, .sides = 8},
    {.amount = 3, .sides = 12},
    {.amount = 4, .sides = 10},
    {.amount = 4, .sides = 12},
    {.amount = 3, .sides = 20},
    {.amount = 4, .sides = 20}};

u8_t random_range(u8_t upper_bound);

const Die *get_die(stat_t stat)
{
    stat = MAX(1, stat);
    stat = MIN(stat, N_DICE);

    return dice + (stat - 1);
}

u8_t roll_die(const Die *die)
{
    u8_t roll = die->amount;
    for (u8_t i = 0; i < die->amount; ++i)
    {
        roll += random_range(die->sides);
    }

    return roll;
}

u8_t random_range(u8_t upper_bound)
{
    return rand() % (upper_bound + 1);
}