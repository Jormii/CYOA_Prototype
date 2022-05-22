#ifndef SKILL_COMMON_H
#define SKILL_COMMON_H

#include "unit.h"
#include "types.h"

typedef enum SkillPriority_en
{
    PRIORITY_LOWEST,
    PRIORITY_MID_LOW,
    PRIORITY_NORMAL,
    PRIORITY_MID_HIGH,
    PRIORITY_HIGHEST
} SkillPriority;

typedef struct SkillInfo_st
{
    size_t id;
    const char *name;
    stamina_t cost;
    SkillPriority priority;
} SkillInfo;

#endif