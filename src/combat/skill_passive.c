#include "skill_passive.h"

void passive_skill_initialize(PassiveSkill *skill, const PassiveSkillMetadata *metadata)
{
    skill->metadata = metadata;
    skill->buffer = NULL;
    if (metadata->initialize_cb != NULL)
    {
        metadata->initialize_cb(skill);
    }
}

void passive_skill_deinitialize(PassiveSkill *skill)
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