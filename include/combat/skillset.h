#ifndef SKILLSET_H
#define SKILLSET_H

#include "types.h"
#include "skill.h"

typedef struct SkillSetMetadata_st
{
    size_t n_skills;
    SkillMetadata **skills_metadata;
} SkillSetMetadata;

typedef struct SkillSet_st
{
    const SkillSetMetadata *metadata;
    Skill *skills;
} SkillSet;

void skillset_initialize(SkillSet *skillset, const SkillSetMetadata *metadata);
void skillset_deinitialize(SkillSet *skillset);

#endif