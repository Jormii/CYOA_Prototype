#ifndef PASSIVE_SKILL_H
#define PASSIVE_SKILL_H

#include "skill.h"

typedef struct PassiveSkillMetadata_st
{
    SkillMetadata metadata;
} PassiveSkillMetadata;

typedef struct PassiveSkill_st
{
    const PassiveSkillMetadata *metadata;
} PassiveSkill;

#endif