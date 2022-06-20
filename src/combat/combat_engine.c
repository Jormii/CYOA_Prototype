#include "log.h" // TODO: Remove
#include "utils.h"
#include "combat_damage.h"
#include "combat_engine.h"

void broadcast_event_to_unit(SkillCommand *event_command, CombatTeam *combat_team, combat_slot_t slot);

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

    combat_engine.on_event_cb = NULL;

    combat_damage_initialize();
}

void combat_engine_broadcast_engine_event(CombatEvent event)
{
    SkillCommand event_command = {
        .skill = NULL,
        .event = event};
    combat_engine_broadcast_event(&event_command);
}

void combat_engine_broadcast_event(SkillCommand *event_command)
{
    LOG("-- Event %u --\n", event_command->event);

    if (combat_engine.on_event_cb != NULL)
    {
        combat_engine.on_event_cb(event_command);
    }

    // Broadcast to units
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        broadcast_event_to_unit(event_command, &(combat_engine.players_team), slot);
    }
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        broadcast_event_to_unit(event_command, &(combat_engine.enemy_team), slot);
    }

    combat_engine_execute_queue();
}

void combat_engine_add_command_to_queue(const SkillCommand *command)
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
    /**
     * Might be called from multiple places, "stacking" the calls
     * Before executing a command, its execution is broadcasted. This might trigger
     *  events that cause the queue to be modified, adding skills with higher priority.
     *  Commands are tagged as broadcast or executed to avoid the same command
     *  from triggering twice
     */

    FixedList *queue = &(combat_engine.skills_queue.fixed_list);
    fixed_list_bubble_sort(queue, queue_compare);

    SkillCommand broadcast_command;
    while (queue->length != 0)
    {
        // Peek
        SkillCommand *command = (SkillCommand *)(fixed_list_get(queue, 0));

        if (combat_identifier_still_deployed(&(command->caster)))
        {
            const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));

            // Broadcast execution
            if (!command->broadcasted)
            {
                LOG("%ls: %ls | %u (Broadcast)\n",
                    caster->unit->name, command->skill->metadata->name, command->event);

                command->broadcasted = TRUE;
                combat_engine_format_passive_command(
                    command->skill, &(command->caster), &(command->target),
                    COMBAT_EVENT_SKILL_EXECUTION, command, &broadcast_command);
                combat_engine_broadcast_event(&broadcast_command);
            }
            else
            {
                return; // See comment atop the function
            }

            // Execute
            if (!command->executed)
            {
                LOG("%ls: %ls | %u (Execution)\n",
                    caster->unit->name, command->skill->metadata->name, command->event);

                command->executed = TRUE;
                command->skill->metadata->execute_cb(command);
            }
            else
            {
                return; // See comment atop the function
            }

            if (!combat_event_is_engine_event(command->event))
            {
                // TODO: Checking will depend on targets
                // Check if this command killed the target
                CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
                if (target != NULL && !unit_is_alive(target->unit) && combat_unit_tag_as_dead(target))
                {
                    SkillCommand event_command;
                    combat_engine_format_passive_command(
                        command->skill, &(command->caster), &(command->target),
                        COMBAT_EVENT_UNIT_DIED, command, &event_command);
                    combat_engine_broadcast_event(&event_command);
                }
            }

            LOG("%ls: %ls | %u (END)\n",
                caster->unit->name, command->skill->metadata->name, command->event);
        }

        // Remove top
        fixed_list_remove(queue, 0);
    }

    LOG("-- End of queue execution --\n\n");
}

void combat_engine_format_active_command(
    Skill *skill, const CombatIdentifier *caster, const CombatIdentifier *target,
    SkillCommand *out_command)
{
    out_command->skill = skill;
    out_command->event = COMBAT_EVENT_NONE;
    combat_identifier_copy(caster, &(out_command->caster));
    combat_identifier_copy(target, &(out_command->target));

    out_command->broadcasted = FALSE;
    out_command->executed = FALSE;
}

void combat_engine_format_passive_command(
    Skill *skill, const CombatIdentifier *caster, const CombatIdentifier *target,
    CombatEvent event, const SkillCommand *cause, SkillCommand *out_command)
{
    out_command->skill = skill;
    out_command->event = event;
    combat_identifier_copy(caster, &(out_command->caster));
    combat_identifier_copy(target, &(out_command->target));

    SkillCommandCause *out_cause = &(out_command->cause);
    out_cause->skill = cause->skill;
    out_cause->event = cause->event;
    combat_identifier_copy(&(cause->caster), &(out_cause->caster));

    out_command->broadcasted = FALSE;
    out_command->executed = FALSE;
}

void broadcast_event_to_unit(SkillCommand *event_command, CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *caster = combat_team_get_combat_unit(combat_team, slot);
    if (caster == NULL)
    {
        return;
    }

    CombatIdentifier caster_identifier = {
        .unit_id = caster->unit->id,
        .unit_slot = slot,
        .combat_team = combat_team};
    combat_identifier_copy(&caster_identifier, &(event_command->caster));

    // Broadcast to skills
    SkillSet *skillset = &(caster->skillset);
    for (size_t i = 0; i < skillset->metadata->n_skills; ++i)
    {
        event_command->skill = skillset->skills + i;

        SkillTrigger_fp trigger_cb = event_command->skill->metadata->trigger_cb;
        if (trigger_cb != NULL && trigger_cb(event_command))
        {
            combat_engine_add_command_to_queue(event_command);
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