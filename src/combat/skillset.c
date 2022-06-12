#include "skillset.h"

void skillset_initialize(SkillSet *skillset, const SkillSetTemplate *template)
{
    // Init actives
    skillset->n_actives = template->n_actives;
    skillset->actives = malloc(skillset->n_actives * sizeof(ActiveSkill));
    for (size_t i = 0; i < skillset->n_actives; ++i)
    {
        active_skill_initialize(skillset->actives + i, template->actives_metadata[i]);
    }

    // Init passives
    skillset->n_passives = template->n_passives;
    skillset->passives = malloc(skillset->n_passives * sizeof(PassiveSkill));
    for (size_t i = 0; i < skillset->n_passives; ++i)
    {
        passive_skill_initialize(skillset->passives + i, template->passives_metadata[i]);
    }
}

void skillset_deinitialize(SkillSet *skillset)
{
    // Deinit actives
    for (size_t i = 0; i < skillset->n_actives; ++i)
    {
        active_skill_deinitialize(skillset->actives + i);
    }
    free(skillset->actives);

    // Deinit passives
    for (size_t i = 0; i < skillset->n_passives; ++i)
    {
        passive_skill_deinitialize(skillset->passives + i);
    }
    free(skillset->passives);
}