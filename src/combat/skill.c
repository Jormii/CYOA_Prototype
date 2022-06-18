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

bool_t skill_command_caster_is_cause_of_event(const SkillCommand *command)
{
    const CombatIdentifier *caster = &(command->caster);
    const CombatEventSource *cause = &(command->cause);
    if (combat_event_is_engine_event(cause->event))
    {
        return FALSE;
    }

    return caster->unit_id == cause->caused_by.unit_id;
}