#include "skill_active.h"

void active_skill_initialize(ActiveSkill *skill, const ActiveSkillMetadata *metadata)
{
    skill->metadata = metadata;
    skill->buffer = NULL;
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