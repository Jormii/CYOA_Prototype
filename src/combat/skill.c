#include "skill.h"

void skill_initialize(Skill *skill, const SkillMetadata *metadata)
{
    skill->metadata = metadata;
    skill->skill_buffer = NULL;

    SkillInitialize_fp init_cb = metadata->initialization.initialize_cb;
    if (init_cb != NULL)
    {
        skill->skill_buffer = malloc(metadata->initialization.skill_buffer_size);
        init_cb(skill->skill_buffer);
    }
}

void skill_deinitialize(Skill *skill)
{
    SkillDeinitialize_fp deinit_cb = skill->metadata->deinitialize_cb;
    if (deinit_cb != NULL)
    {
        deinit_cb(skill->skill_buffer);
    }

    if (skill->skill_buffer != NULL)
    {
        free(skill->skill_buffer);
    }
}