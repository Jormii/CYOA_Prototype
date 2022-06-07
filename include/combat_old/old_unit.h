#ifndef UNIT_H
#define UNIT_H

#include "types.h"
#include "attribute.h"
#include "dynamic_list.h"

typedef u8_t health_t;
typedef u8_t stamina_t;

struct Species_st;
struct SkillSet_st;

typedef struct Unit_st
{
    size_t id;
    char *name;
    const struct Species_st *species;
    const struct SkillSet_st *skillset;
    health_t hp;
    stamina_t stamina;
    attribute_t attributes[ATTR_COUNT];
    DynamicList special_conditions;
} Unit;

#endif