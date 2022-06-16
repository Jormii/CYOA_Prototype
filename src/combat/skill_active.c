#include "skill_active.h"

void active_skill_initialize(ActiveSkill *skill, const ActiveSkillMetadata *metadata)
{
    skill->metadata = metadata;
    skill->buffer = NULL;

    skill->passives = NULL;
    if (metadata->n_passives != 0)
    {
        skill->passives = malloc(metadata->n_passives * sizeof(PassiveSkill));
        for (size_t i = 0; i < metadata->n_passives; ++i)
        {
            passive_skill_initialize(skill->passives + i, metadata->passives[i]);
        }
    }

    // Passives don't know they belong to an active. Initialize the active
    //  afterwards in case the active needs to update data on the passives
    if (metadata->initialize_cb != NULL)
    {
        metadata->initialize_cb(skill);
    }
}

void active_skill_deinitialize(ActiveSkill *skill)
{
    if (skill->metadata->deinitialize_cb != NULL)
    {
        skill->metadata->deinitialize_cb(skill);
    }

    if (skill->buffer != NULL)
    {
        free(skill->buffer);
    }
}