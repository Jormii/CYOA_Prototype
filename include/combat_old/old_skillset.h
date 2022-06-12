#ifndef SKILLSET_H
#define SKILLSET_H

#include "types.h"
#include "active_skill.h"
#include "passive_skill.h"

typedef struct SkillSetTemplate_st
{
    size_t n_actives;
    size_t n_passives;
    ActiveSkillInfo **actives_info;
    PassiveSkillInfo **passives_info;
} SkillSetTemplate;

typedef struct SkillSet_st
{
    size_t n_actives;
    size_t n_passives;
    ActiveSkill *actives;
    PassiveSkill *passives;
} SkillSet;

SkillSet *skillset_init(const SkillSetTemplate *template);
void skillset_deinit(SkillSet *skillset);

#endif