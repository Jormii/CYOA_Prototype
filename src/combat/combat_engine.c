#include "utils.h"
#include "combat_engine.h"

// TODO: Redo at some point
typedef struct EventCommand_st
{
    bool_t is_skill;
    union
    {
        PassiveSkillCommand skill_cmd;
        SpecialConditionCommand condition_cmd;
    };
} EventCommand;

void combat_team_initialize(CombatTeam *combat_team, bool_t is_players_team);

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot);

bool_t active_queue_compare(const byte_t *first, const byte_t *second);
bool_t passive_queue_compare(const byte_t *first, const byte_t *second);
bool_t queue_compare(const SkillMetadata *first_metadata, const CombatDescriptor *first_caster,
                     const SkillMetadata *second_metadata, const CombatDescriptor *second_caster);

CombatUnit *combat_team_get_combat_unit(CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    if (cu->slot_occupied)
    {
        return cu;
    }
    else
    {
        return NULL;
    }
}

combat_slot_t combat_team_count_available_units(const CombatTeam *combat_team)
{
    combat_slot_t count = 0;
    for (combat_slot_t i = 0; i < MAX_UNITS_IN_TEAM; ++i)
    {
        const Unit *unit = combat_team->team.units + i;
        bool_t available = unit_is_valid(unit) && unit->hp > 0 && !combat_team_unit_is_in_combat(combat_team, unit->id);
        count += available;
    }

    return count;
}

bool_t combat_team_unit_is_in_combat(const CombatTeam *combat_team, size_t unit_id)
{
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team->combat_units + slot;
        if (cu->slot_occupied && cu->unit->id == unit_id)
        {
            return TRUE;
        }
    }

    return FALSE;
}

void ce_initialize()
{
    combat_engine.in_combat = FALSE;
    combat_team_initialize(&(combat_engine.players_team), TRUE);
    combat_team_initialize(&(combat_engine.enemy_team), FALSE);
    fixed_list_init(
        &(combat_engine.active_commands_queue), 2 * MAX_UNITS_IN_COMBAT, sizeof(ActiveSkillCommand));
    dynamic_list_init(
        &(combat_engine.event_commands_queue), 2 * 2 * MAX_UNITS_IN_COMBAT, 2, sizeof(EventCommand));

    ce_damage_initialize();
}

void ce_choose_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = unit;
    cu->slot_occupied = TRUE;
    skillset_initialize(&(cu->skillset), unit->species->skillset_template);
}

void ce_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = NULL;
    cu->slot_occupied = FALSE;
    skillset_deinitialize(&(cu->skillset));
    fixed_list_clear(&(cu->special_conditions.fixed_list));
}

void ce_broadcast_engine_event(CombatEvent event)
{
    CombatEventSource source = {
        .caused_by_engine = TRUE,
        .event = event};
    ce_broadcast_event(&source);
}

void ce_broadcast_event(CombatEventSource *source)
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

    // Execute passive commands
    // TODO: Check if the queue is being executed?
    FixedList *queue = &(combat_engine.event_commands_queue.fixed_list);
    fixed_list_bubble_sort(queue, passive_queue_compare); // TODO: New passives might be added to the queue

    EventCommand command;
    while (queue->length != 0)
    {
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, queue->element_size);
        fixed_list_remove(queue, 0);

        if (command.is_skill)
        {
            command.skill_cmd.passive->metadata->execute_cb(&(command.skill_cmd));
        }
        else
        {
            command.condition_cmd.condition->metadata->execute_cb(&(command.condition_cmd));
        }
    }
}

void ce_add_active_to_queue(const ActiveSkillCommand *command)
{
    fixed_list_append(&(combat_engine.active_commands_queue), (byte_t *)command);
}

void ce_remove_queue_tail()
{
    size_t l = combat_engine.active_commands_queue.length;
    if (l != 0)
    {
        fixed_list_remove(&(combat_engine.active_commands_queue), l - 1);
    }
}

void ce_execute_queue()
{
    FixedList *queue = &(combat_engine.active_commands_queue);
    fixed_list_bubble_sort(queue, active_queue_compare);

    ActiveSkillCommand command;
    while (queue->length != 0)
    {
        // Pop
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, queue->element_size);
        fixed_list_remove(queue, 0);

        // Execute
        command.active->metadata->execute_cb(&command);
    }
}

void ce_apply_condition(size_t cause_id, CombatUnit *target, SpecialConditionMetadata *metadata)
{
    SpecialCondition condition = {
        .cause_id = cause_id,
        .metadata = metadata};
    dynamic_list_append(&(target->special_conditions), (byte_t *)&condition);
}

void combat_team_initialize(CombatTeam *combat_team, bool_t is_players_team)
{
    team_default_initialization(&(combat_team->team));
    combat_team->is_players_team = is_players_team;

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        CombatUnit *cu = combat_team->combat_units + slot;
        cu->unit = NULL;
        cu->slot_occupied = FALSE;
        dynamic_list_init(&(cu->special_conditions), 2, 1, sizeof(SpecialCondition));
    }
}

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *caster = combat_team_get_combat_unit(combat_team, slot);
    if (caster == NULL)
    {
        return;
    }

    EventCommand event_cmd;

    // Broadcast to passives
    event_cmd.is_skill = TRUE;
    event_cmd.skill_cmd.caster.unit_id = caster->unit->id;
    event_cmd.skill_cmd.caster.unit_slot = slot;
    event_cmd.skill_cmd.caster.combat_team = combat_team;
    event_cmd.skill_cmd.source = source;

    SkillSet *skillset = &(caster->skillset);
    for (size_t i = 0; i < skillset->n_passives; ++i)
    {
        PassiveSkill *skill = skillset->passives + i;
        if (skill->metadata->triggers & source->event)
        {
            event_cmd.skill_cmd.passive = skill;
            dynamic_list_append(&(combat_engine.event_commands_queue), (byte_t *)&event_cmd);
        }
    }

    // Broadcast to conditions
    event_cmd.is_skill = FALSE;
    event_cmd.condition_cmd.affected.unit_id = caster->unit->id;
    event_cmd.condition_cmd.affected.unit_slot = slot;
    event_cmd.condition_cmd.affected.combat_team = combat_team;
    event_cmd.condition_cmd.source = source;

    FixedList *conditions = &(caster->special_conditions.fixed_list);
    for (size_t i = 0; i < conditions->length; ++i)
    {
        SpecialCondition *condition = (SpecialCondition *)fixed_list_get(conditions, i);
        if (condition->metadata->triggers & source->event)
        {
            event_cmd.condition_cmd.condition = condition;
            dynamic_list_append(&(combat_engine.event_commands_queue), (byte_t *)&event_cmd);
        }
    }
}

bool_t active_queue_compare(const byte_t *first, const byte_t *second)
{
    ActiveSkillCommand *first_cmd = (ActiveSkillCommand *)first;
    ActiveSkillCommand *second_cmd = (ActiveSkillCommand *)second;

    return queue_compare(&(first_cmd->active->metadata->metadata), &(first_cmd->caster),
                         &(second_cmd->active->metadata->metadata), &(second_cmd->caster));
}

bool_t passive_queue_compare(const byte_t *first, const byte_t *second)
{
    PassiveSkillCommand *first_cmd = (PassiveSkillCommand *)first;
    PassiveSkillCommand *second_cmd = (PassiveSkillCommand *)second;

    return queue_compare(&(first_cmd->passive->metadata->metadata), &(first_cmd->caster),
                         &(second_cmd->passive->metadata->metadata), &(second_cmd->caster));
}

bool_t queue_compare(const SkillMetadata *first_metadata, const CombatDescriptor *first_caster,
                     const SkillMetadata *second_metadata, const CombatDescriptor *second_caster)
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