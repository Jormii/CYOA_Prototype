#include "skill_includes.h"

bool_t goat_unstoppable_trigger(const SkillCommand *command)
{
    if (command->event != COMBAT_EVENT_UNIT_DIED)
    {
        return FALSE;
    }

    return skill_command_caster_is_cause_of_event(command) &&
           skill_command_cause_is_active(&(command->cause));
}

void goat_unstoppable_execute(SkillCommand *command)
{
    const SkillCommandCause *cause = &(command->cause);

    const CombatIdentifier *target = &(command->target);
    combat_slot_t new_target_slot = (target->unit_slot + 1) % MAX_UNITS_IN_COMBAT;
    CombatUnit *target_unit = combat_team_get_combat_unit(
        target->combat_team, new_target_slot);
    if (target_unit == NULL)
    {
        return;
    }

    CombatIdentifier new_target = {
        .unit_id = target_unit->unit->id,
        .unit_slot = new_target_slot,
        .combat_team = target->combat_team};

    SkillCommand active_command;
    combat_engine_format_active_command(
        cause->skill, &(cause->caster), &new_target, &active_command);
    combat_engine_add_command_to_queue(&active_command);

    const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    tb_printf(skills_buffer, 0x00FFFFFF, L"%ls keeps going!\n", caster->unit->name);
}

SkillMetadata goat_unstoppable_meta = {
    .id = SKILL_ID_GOAT_UNSTOPPABLE_ID,
    .type = SKILL_TYPE_PASSIVE,
    .attribute = ATTR_AGILITY,
    .cost = 0,
    .name = L"Unstoppable (P)",
    .description = L"If this unit's attack kills its target...",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_unstoppable_trigger,
    .execute_cb = goat_unstoppable_execute};