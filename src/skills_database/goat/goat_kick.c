#include "all_skills.h"

void goat_kick_execute(SkillCommand *command)
{
    // TODO
    ce_damage_declare_attack(&(command->caster), &(command->target));
    ce_damage_perform();
}

SkillMetadata goat_kick_meta = {
    .id = SKILL_ID_GOAT_KICK_ID,
    .type = SKILL_TYPE_ACTIVE_NOT_SELF,
    .cost = 0,
    .name = L"Kick",
    .description = L"Kicks a single unit, dealing damage to it",
    .priority = SKILL_PRIORITY_AVERAGE,
    .triggers = COMBAT_EVENT_NONE,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .execute_cb = goat_kick_execute};