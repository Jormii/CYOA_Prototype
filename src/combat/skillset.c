#include <stdlib.h>

#include "skillset.h"

void init_actives(const SkillSetTemplate *template, SkillSet *new);
void init_passives(const SkillSetTemplate *template, SkillSet *new);

void deinit_actives(SkillSet *skill_set);
void deinit_passives(SkillSet *skill_set);

SkillSet *skillset_init(const SkillSetTemplate *template)
{
    SkillSet *new = malloc(sizeof(SkillSet));
    init_actives(template, new);
    init_passives(template, new);

    return new;
}

void skillset_deinit(SkillSet *skillset)
{
    deinit_actives(skillset);
    deinit_passives(skillset);
    free(skillset);
}

void init_actives(const SkillSetTemplate *template, SkillSet *new)
{
    new->n_actives = template->n_actives;
    new->actives = malloc(template->n_actives * sizeof(ActiveSkill));
    for (size_t i = 0; i < template->n_actives; ++i)
    {
        ActiveSkill *skill = new->actives + i;
        ActiveSkillInfo *skill_info = template->actives_info[i];

        skill->active_info = skill_info;
        skill->active_buffer = NULL;
        skill_info->init_cb(new->actives + i, skill_info);
    }
}

void init_passives(const SkillSetTemplate *template, SkillSet *new)
{
    new->n_passives = template->n_passives;
    new->passives = malloc(template->n_passives * sizeof(PassiveSkill));
    for (size_t i = 0; i < template->n_passives; ++i)
    {
        PassiveSkill *skill = new->passives + i;
        PassiveSkillInfo *skill_info = template->passives_info[i];

        skill->passive_info = skill_info;
        skill->passive_buffer = NULL;
        skill_info->init_cb(new->passives + i, skill_info);
    }
}

void deinit_actives(SkillSet *skill_set)
{
    for (size_t i = 0; i < skill_set->n_actives; ++i)
    {
        ActiveSkill *skill = skill_set->actives + i;
        skill->active_info->deinit_cb(skill);

        free(skill);
    }
    free(skill_set->actives);
}

void deinit_passives(SkillSet *skill_set)
{
    for (size_t i = 0; i < skill_set->n_actives; ++i)
    {
        PassiveSkill *skill = skill_set->passives + i;
        skill->passive_info->deinit_cb(skill);

        free(skill);
    }
    free(skill_set->passives);
}