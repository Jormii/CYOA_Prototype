#include "log.h"
#include "utils.h"
#include "combat_damage.h"
#include "combat_engine.h"

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot);

bool_t queue_compare(const byte_t *first, const byte_t *second);
bool_t skill_compare(const SkillMetadata *first_metadata, const CombatIdentifier *first_caster,
                     const SkillMetadata *second_metadata, const CombatIdentifier *second_caster);

void combat_engine_initialize()
{
    combat_engine.in_combat = FALSE;
    combat_engine.executing_queue = FALSE;
    combat_team_initialize(&(combat_engine.players_team));
    combat_team_initialize(&(combat_engine.enemy_team));
    dynamic_list_init(
        &(combat_engine.skills_queue), 2 * 2 * MAX_UNITS_IN_COMBAT, 2,
        sizeof(SkillCommand));

    combat_damage_initialize();
}

void combat_engine_broadcast_engine_event(CombatEvent event)
{
    CombatEventSource source = {
        .event = event};
    combat_engine_broadcast_event(&source);
}

void combat_engine_broadcast_event(CombatEventSource *source)
{
    // Broadcast to units
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        ce_broadcast_event_to_unit(source, &(combat_engine.players_team), slot);
    }
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        ce_broadcast_event_to_unit(source, &(combat_engine.enemy_team), slot);
    }

    combat_engine_execute_queue();
}

void combat_engine_add_active_to_queue(Skill *skill, const CombatIdentifier *caster, const CombatIdentifier *target)
{
    SkillCommand command = {
        .skill = skill,
        .caster = *caster,
        .target = *target,
        .cause = {
            .event = COMBAT_EVENT_ENGINE_NONE}};
    fixed_list_append(
        &(combat_engine.skills_queue.fixed_list), (byte_t *)&command);
}

void combat_engine_remove_queue_tail()
{
    size_t l = combat_engine.skills_queue.fixed_list.length;
    if (l != 0)
    {
        fixed_list_remove(&(combat_engine.skills_queue.fixed_list), l - 1);
    }
}

void combat_engine_execute_queue()
{
    FixedList *queue = &(combat_engine.skills_queue.fixed_list);
    fixed_list_bubble_sort(queue, queue_compare);

    // TODO: What if a skill with higher priority is added?

    if (combat_engine.executing_queue)
    {
        return;
    }
    combat_engine.executing_queue = TRUE;

    SkillCommand command;
    while (queue->length != 0)
    {
        // Pop
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, queue->element_size);
        fixed_list_remove(queue, 0);

        if (combat_identifier_still_deployed(&(command.caster)))
        {
            // Broadcast execution
            CombatEventSource source = {
                .event = COMBAT_EVENT_SKILL_EXECUTION,
                .skill_id = command.skill->metadata->id,
                .caused_by = command.caster};
            combat_engine_broadcast_event(&source);

            // Execute
            command.skill->metadata->execute_cb(&command);
        }
    }

    combat_engine.executing_queue = FALSE;
}

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *caster = combat_team_get_combat_unit(combat_team, slot);
    if (caster == NULL)
    {
        return;
    }

    SkillCommand command = {
        .skill = NULL, // Modified as skills are iterated
        .caster = {
            .unit_id = caster->unit->id,
            .unit_slot = slot,
            .combat_team = combat_team},
        .cause = *source};

    // Broadcast to skills
    SkillSet *skillset = &(caster->skillset);
    for (size_t i = 0; i < skillset->metadata->n_skills; ++i)
    {
        command.skill = skillset->skills + i;

        SkillTrigger_fp trigger_cb = command.skill->metadata->trigger_cb;
        if (trigger_cb != NULL && trigger_cb(&command))
        {
            dynamic_list_append(&(combat_engine.skills_queue), (byte_t *)&command);
        }
    }
}

bool_t queue_compare(const byte_t *first, const byte_t *second)
{
    SkillCommand *first_cmd = (SkillCommand *)first;
    SkillCommand *second_cmd = (SkillCommand *)second;

    return skill_compare(first_cmd->skill->metadata, &(first_cmd->caster),
                         second_cmd->skill->metadata, &(second_cmd->caster));
}

bool_t skill_compare(const SkillMetadata *first_metadata, const CombatIdentifier *first_caster,
                     const SkillMetadata *second_metadata, const CombatIdentifier *second_caster)
{
    if (first_metadata->priority > second_metadata->priority)
    {
        return TRUE;
    }
    else if (first_metadata->priority < second_metadata->priority)
    {
        return FALSE;
    }

    const CombatUnit *first_unit = combat_team_get_combat_unit(first_caster->combat_team, first_caster->unit_slot);
    const CombatUnit *second_unit = combat_team_get_combat_unit(second_caster->combat_team, second_caster->unit_slot);
    if (first_unit == NULL || second_unit == NULL)
    {
        // It's irrelevant. Return TRUE to not swap them
        return TRUE;
    }

    return unit_calculate_stat(first_unit->unit, STAT_SPEED) >= unit_calculate_stat(second_unit->unit, STAT_SPEED);
}