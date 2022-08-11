#ifndef UNIT_H
#define UNIT_H

#include "types.h"
#include "species.h"
#include "attribute.h"

#define UNDEFINED_UNIT_ID 0

typedef struct Unit_st
{
    size_t id;
    wchar_t *name;
    health_t hp;
    stamina_t stamina;
    attribute_t attributes[ATTR_COUNT];
    const Species *species;
} Unit;

void unit_initialize(Unit *unit);

bool_t unit_is_valid(const Unit *unit);
bool_t unit_is_alive(const Unit *unit);
stat_t unit_calculate_stat(const Unit *unit, Stat stat);
bool_t unit_knows_skill(const Unit *unit, const SkillMetadata *skill_meta);

#endif