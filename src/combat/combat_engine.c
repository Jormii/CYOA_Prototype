#include "utils.h"
#include "skillset.h"
#include "combat_engine.h"
#include "special_condition.h"

void ce_init_team(Team *team, bool_t is_players_team);
void ce_starting_units(Team *team);

void ce_team_loop(Team *team);
void ce_sort_command_queue();
void ce_execute_command_queue();
void ce_update_combat_state();

void ce_replace_unit(Team *team, slot_t slot);
void ce_engine_broadcast(CombatEvent event);
void ce_broadcast_to_passives(CombatEventSource *source, Unit *unit, Team *team, slot_t slot);
void ce_broadcast_to_special_conditions(CombatEventSource *source, Unit *unit, Team *team, slot_t slot);

void ce_execute_passive(CombatEventSource *source, PassiveSkill *skill, Unit *unit, Team *team, slot_t slot);
bool_t ce_can_execute_command(ActiveSkillCommand *command);
bool_t faster_command(size_t command_i, size_t command_j);
void swap_commands(size_t command_i, size_t command_j);

#pragma region Header functions

void ce_init()
{
    combat_engine.in_combat = FALSE;
    ce_init_team(&(combat_engine.players_team), TRUE);
    ce_init_team(&(combat_engine.enemy_team), FALSE);
    fixed_list_init(&(combat_engine.active_commands), COMMAND_QUEUE_LENGTH,
                    sizeof(ActiveSkillCommand));
    dynamic_list_init(&(combat_engine.passive_commands), PASSIVE_COMMAND_QUEUE_LENGTH,
                      DYNAMIC_LIST_INCREMENT_DOUBLE, sizeof(PassiveSkillCommand));
}

void ce_start_combat()
{
    combat_engine.in_combat = TRUE;
    ce_starting_units(&(combat_engine.players_team));
    ce_starting_units(&(combat_engine.enemy_team));
}

void ce_combat_loop()
{
    ce_engine_broadcast(EVENT_START_OF_TURN);

    ce_team_loop(&(combat_engine.players_team));
    ce_team_loop(&(combat_engine.enemy_team));

    ce_sort_command_queue();
    ce_execute_command_queue();
    ce_update_combat_state();

    ce_engine_broadcast(EVENT_END_OF_TURN);

    // TODO: Remove
    ce_engine_broadcast(EVENT_START_OF_TURN);
}

void ce_push_active_command(const ActiveSkillCommand *command)
{
    fixed_list_append(&(combat_engine.active_commands), (byte_t *)command);
}

void ce_broadcast_event(CombatEventSource *source)
{
    ce_on_event(source);
    ce_broadcast_event_to_team(source, &(combat_engine.players_team));
    ce_broadcast_event_to_team(source, &(combat_engine.enemy_team));

    // Buffer
    PassiveSkillCommand *command_buffer = malloc(sizeof(PassiveSkillCommand));

    FixedList *queue = &(combat_engine.passive_commands.fixed_list);
    while (queue->length != 0)
    {
        size_t index = queue->length - 1;
        byte_t *buffer = fixed_list_get(queue, index);

        /* 
        PROBLEM: When a passive is executed, that passive may trigger another
            event, meaning the queue will see itself modified.
            Remove the command from the queue before executing
        */
        copy_buffer(buffer, (byte_t *)command_buffer, sizeof(PassiveSkillCommand));
        fixed_list_remove(queue, index);

        PassiveSkillCommand *command = command_buffer;
        PassiveSkill *skill = command->passive_skill;
        skill->passive_info->execute_cb(command);
    }

    free(command_buffer);
}

void ce_broadcast_event_to_team(CombatEventSource *source, Team *team)
{
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        ce_broadcast_event_to_slot(source, team, slot);
    }
}

void ce_broadcast_event_to_slot(CombatEventSource *source, Team *team, slot_t slot)
{
    Unit *unit = team_get_unit(team, slot);
    if (unit == NULL)
    {
        return;
    }

    ce_broadcast_to_passives(source, unit, team, slot);
    ce_broadcast_to_special_conditions(source, unit, team, slot);
}

#pragma endregion

#pragma region Source functions

void ce_starting_units(Team *team)
{
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        if (!(team->is_players_team))
        {
            team->in_combat[slot] = slot;
        }
        else
        {
            ce_replace_unit(team, slot);
        }
    }
}

void ce_team_loop(Team *team)
{
    for (int slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        Unit *unit = team_get_unit(team, slot);
        if (unit == NULL)
        {
            continue;
        }

        ActiveSkillCommand command = {
            .active_skill = ce_ask_for_skill(unit),
            .caster = {
                .unit_id = unit->id,
                .team = team,
                .slot = slot}};

        ce_ask_for_skills_target(command.active_skill, &(command.target));
        ce_push_active_command(&command);
    }
}

void ce_sort_command_queue()
{
    /***
     * Because of how the list works, the list is sorted from slowest to fastest
     * and the queue is later popped
     */

    bool_t swapped = FALSE;
    size_t n = combat_engine.active_commands.length;
    for (size_t i = 0; i < (n - 1); ++i)
    {
        swapped = FALSE;
        for (size_t j = 0; j < (n - 1 - i); ++j)
        {
            if (faster_command(j, j + 1))
            {
                swap_commands(j, j + 1);
                swapped = TRUE;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}

void ce_execute_command_queue()
{
    ce_engine_broadcast(EVENT_BEFORE_EXECUTION);

    FixedList *queue = &(combat_engine.active_commands);
    while (queue->length != 0)
    {
        size_t index = queue->length - 1;
        ActiveSkillCommand *command = (ActiveSkillCommand *)(fixed_list_get(queue, index));
        if (ce_can_execute_command(command))
        {
            stamina_t cost = command->active_skill->active_info->execute_cb(command);
            if (cost != 0)
            {
                UnitSource *source = &(command->caster);
                Unit *unit = team_get_unit(source->team, source->slot);
                if (unit != NULL)
                {
                    unit->stamina -= cost;
                }
            }
        }
        fixed_list_remove(queue, index);
    }
}

void ce_update_combat_state()
{
    // TODO: Remove
    combat_engine.in_combat = FALSE;
    // END TODO
}

void ce_replace_unit(Team *team, slot_t slot)
{
    // TODO
    team->in_combat[slot] = slot;
}

void ce_engine_broadcast(CombatEvent event)
{
    CombatEventSource source = {
        .event = event,
        .caused_by_engine = TRUE};

    ce_broadcast_event(&source);
}

void ce_broadcast_to_passives(CombatEventSource *source, Unit *unit, Team *team, slot_t slot)
{
    CombatEvent event = source->event;
    const SkillSet *skillset = unit->skillset;
    for (size_t i = 0; i < skillset->n_passives; ++i)
    {
        PassiveSkill *skill = skillset->passives + i;
        const PassiveSkillInfo *info = skill->passive_info;
        if ((info->triggers & event) && info->can_be_used_cb(skill, unit))
        {
            ce_execute_passive(source, skill, unit, team, slot);
        }
    }
}

void ce_broadcast_to_special_conditions(CombatEventSource *source, Unit *unit, Team *team, slot_t slot)
{
    CombatEvent event = source->event;

    size_t i = 0;
    size_t *length = &(unit->special_conditions.fixed_list.length);
    while (i < (*length))
    {
        // TODO: Traversal is inefficient
        byte_t *buffer = fixed_list_get(&(unit->special_conditions.fixed_list), i);
        SpecialCondition *special_condition = (SpecialCondition *)buffer;
        const SpecialConditionInfo *condition_info = special_condition->condition_info;

        bool_t increment = TRUE;
        if (condition_info->triggers & event)
        {
            SpecialConditionCommand command = {
                .affected = {
                    .unit_id = unit->id,
                    .team = team,
                    .slot = slot},
                .special_condition = special_condition,
                .source = source};

            condition_info->execute_cb(&command);
            if (condition_info->wore_off_cb(special_condition))
            {
                fixed_list_remove(&(unit->special_conditions.fixed_list), i);
                increment = FALSE;
            }
        }

        i += increment;
    }
}

void ce_execute_passive(CombatEventSource *source, PassiveSkill *skill, Unit *unit, Team *team, slot_t slot)
{
    // TODO: Functions naming is not correct
    PassiveSkillCommand command = {
        .caster = {
            .unit_id = unit->id,
            .team = team,
            .slot = slot},
        .passive_skill = skill,
        .source = source};

    dynamic_list_append(&(combat_engine.passive_commands), (byte_t *)(&command));
}

bool_t ce_can_execute_command(ActiveSkillCommand *command)
{
    const UnitSource *source = &(command->caster);
    const Unit *caster = team_get_unit(source->team, source->slot);
    if (caster == NULL || source->unit_id != caster->id)
    {
        return FALSE;
    }

    const ActiveSkill *skill = command->active_skill;
    return caster->stamina > 0 && skill->active_info->can_be_used_cb(skill, caster);
}

bool_t faster_command(size_t command_i, size_t command_j)
{
    byte_t *buffer_i = fixed_list_get(&(combat_engine.active_commands), command_i);
    byte_t *buffer_j = fixed_list_get(&(combat_engine.active_commands), command_j);

    ActiveSkillCommand *ci = (ActiveSkillCommand *)buffer_i;
    ActiveSkillCommand *cj = (ActiveSkillCommand *)buffer_j;
    const SkillInfo *ci_info = &(ci->active_skill->active_info->skill_info);
    const SkillInfo *cj_info = &(cj->active_skill->active_info->skill_info);
    if (ci_info->priority > cj_info->priority)
    {
        return TRUE;
    }
    else if (ci_info->priority < cj_info->priority)
    {
        return FALSE;
    }

    const UnitSource *ci_source = &(ci->caster);
    const UnitSource *cj_source = &(cj->caster);

    const Unit *ci_unit = team_get_unit(ci_source->team, ci_source->slot);
    const Unit *cj_unit = team_get_unit(cj_source->team, cj_source->slot);
    return ci_unit->attributes[ATTR_AGILITY] > cj_unit->attributes[ATTR_AGILITY];
}

void swap_commands(size_t command_i, size_t command_j)
{
    byte_t *buffer_i = fixed_list_get(&(combat_engine.active_commands), command_i);
    byte_t *buffer_j = fixed_list_get(&(combat_engine.active_commands), command_j);
    swap_buffer(buffer_i, buffer_j, combat_engine.active_commands.element_size);
}

#pragma endregion