#ifndef SKILL_H
#define SKILL_H

#include "types.h"

typedef enum SkillPriority_en
{
    SKILL_PRIORITY_LOWEST,
    SKILL_PRIORITY_LOW,
    SKILL_PRIORITY_AVERAGE,
    SKILL_PRIORITY_HIGH,
    SKILL_PRIORITY_HIGHEST
} SkillPriority;

typedef struct SkillMetadata_st
{
    const wchar_t *name;
    stamina_t cost;
    SkillPriority priority;
} SkillMetadata;

#endif