#ifndef STAT_H
#define STAT_H

#include "types.h"

typedef u8_t stat_t;

typedef enum Stat_en
{
    STAT_HEALTH,
    STAT_STAMINA,
    STAT_DAMAGE,
    STAT_ENDURANCE,
    STAT_SPEED,
    STAT_COUNT
} Stat;

#endif