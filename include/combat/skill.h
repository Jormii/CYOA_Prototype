#ifndef SKILL_H
#define SKILL_H

#include "types.h"

typedef struct SkillMetadata_st
{
    const wchar_t *name;
    stamina_t cost;
} SkillMetadata;

#endif