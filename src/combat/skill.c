#include "skill.h"
#include "combat_unit.h"

bool_t skill_metadata_is_active(const SkillMetadata *metadata)
{
    return metadata->type > _SKILL_TYPE_ACTIVE_;
}

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

bool_t skill_command_is_active(const SkillCommand *command)
{
    return command->event == COMBAT_EVENT_NONE;
}

bool_t skill_command_caster_is_cause_of_event(const SkillCommand *command)
{
    if (command->event == COMBAT_EVENT_NONE)
    {
        return FALSE;
    }

    return combat_identifier_are_same_unit(&(command->caster), &(command->cause.caster));
}