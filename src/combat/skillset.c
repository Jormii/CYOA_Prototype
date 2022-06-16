#include "skillset.h"

void skillset_initialize(SkillSet *skillset, const SkillSetMetadata *metadata)
{
    skillset->metadata = metadata;
    skillset->skills = malloc(metadata->n_skills * sizeof(Skill));

    for (size_t i = 0; i < metadata->n_skills; ++i)
    {
        skill_initialize(skillset->skills + i, metadata->skills_metadata[i]);
    }
}

void skillset_deinitialize(SkillSet *skillset)
{
    for (size_t i = 0; i < skillset->metadata->n_skills; ++i)
    {
        skill_deinitialize(skillset->skills + i);
    }
    free(skillset->skills);

    skillset->metadata = NULL;
    skillset->skills = NULL;
}