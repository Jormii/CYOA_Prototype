#ifndef ACTIVE_SKILL_H
#define ACTIVE_SKILL_H

#include "skill.h"

typedef struct ActiveSkillMetadata_st
{
    SkillMetadata metadata;
} ActiveSkillMetadata;

typedef struct ActiveSkill_st
{
    const ActiveSkillMetadata *metadata;
} ActiveSkill;

#endif