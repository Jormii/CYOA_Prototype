#ifndef SKILLSET_H
#define SKILLSET_H

#include "types.h"
#include "skill_active.h"
#include "skill_passive.h"

typedef struct SkillSetTemplate_st
{
    size_t n_actives;
    size_t n_passives;
    ActiveSkillMetadata **actives_metadata;
    PassiveSkillMetadata **passives_metadata;
} SkillSetTemplate;

typedef struct SkillSet_st
{
    size_t n_actives;
    size_t n_passives;
    ActiveSkill *actives;
    PassiveSkill *passives;
} SkillSet;

void skillset_initialize(SkillSet *skillset, const SkillSetTemplate *template);
void skillset_deinitialize(SkillSet *skillset);

#endif