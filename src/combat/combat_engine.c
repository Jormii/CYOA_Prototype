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
        .caused_by_engine = TRUE,
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

void combat_engine_add_active_to_queue(const SkillCommand *command)
{
    fixed_list_append(
        &(combat_engine.skills_queue.fixed_list), (byte_t *)command);
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
    // TODO: Check if the queue is being executed?
    // TODO: What if the size of the queue increases?
    FixedList *queue = &(combat_engine.skills_queue.fixed_list);
    fixed_list_bubble_sort(queue, queue_compare);

    SkillCommand command;
    CombatEventSource source = {
        .caused_by_engine = FALSE,
        .event = COMBAT_EVENT_SKILL_EXECUTION};
    while (queue->length != 0)
    {
        // Pop
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, queue->element_size);
        fixed_list_remove(queue, 0);

        // Broadcast execution
        source.caused_by = command.caster;
        combat_engine_broadcast_event(&source);

        // Execute
        command.skill->metadata->execute_cb(&command);
    }
}

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *caster = combat_team_get_combat_unit(combat_team, slot);
    if (caster == NULL)
    {
        return;
    }

    SkillCommand command = {
        .caster = {
            .combat_team = combat_team,
            .unit_id = caster->unit->id,
            .unit_slot = slot},
        .event_source = *source};

    // Broadcast to skills
    SkillSet *skillset = &(caster->skillset);
    for (size_t i = 0; i < skillset->metadata->n_skills; ++i)
    {
        Skill *skill = skillset->skills + i;
        if (skill->metadata->triggers & source->event)
        {
            command.skill = skill;
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