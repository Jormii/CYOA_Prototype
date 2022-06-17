#ifndef TYPES_H
#define TYPES_H

#include <wchar.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define FALSE 0
#define TRUE 1

typedef uint8_t bool_t;
typedef uint8_t byte_t;

typedef uint8_t u8_t;
typedef uint32_t u32_t;

// Custom types
typedef u8_t stat_t;
typedef u8_t health_t;
typedef u8_t stamina_t;
typedef u8_t attribute_t;

typedef u8_t combat_slot_t;

typedef u32_t event_flags_t;

#endif