#ifndef UNIT_H
#define UNIT_H

#include "types.h"
#include "attribute.h"

#define UNDEFINED_UNIT_ID 0

typedef u8_t health_t;
typedef u8_t stamina_t;

typedef struct Unit_st
{
    size_t id;
    wchar_t *name;
    health_t hp;
    stamina_t stamina;
    attribute_t attributes[ATTR_COUNT];
} Unit;

void unit_default_initialization(Unit *unit);
bool_t unit_is_valid(const Unit *unit);

#endif