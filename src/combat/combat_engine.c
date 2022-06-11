#include "utils.h"
#include "combat_engine.h"

void combat_team_initialize(CombatTeam *combat_team, bool_t is_players_team);

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot);

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
        &(combat_engine.passive_commands_queue), 2 * 2 * MAX_UNITS_IN_COMBAT, 2, sizeof(PassiveSkillCommand));

    ce_damage_initialize();
}

void ce_choose_unit(CombatTeam *combat_team, Unit *unit, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = unit;
    cu->slot_occupied = TRUE;

    unit->skillset = skillset_initialize(unit->species->skillset_template);
}

void ce_remove_from_combat(CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *cu = combat_team->combat_units + slot;
    cu->unit = NULL;
    cu->slot_occupied = FALSE;

    skillset_deinitialize(cu->unit->skillset);
    cu->unit->skillset = NULL;
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
    // TODO: Sort queue
    PassiveSkillCommand command;
    FixedList *queue = &(combat_engine.passive_commands_queue.fixed_list);
    while (queue->length != 0)
    {
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, queue->element_size);
        fixed_list_remove(queue, 0);

        command.passive->metadata->execute_cb(&command);
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
    // TODO: Sort queue

    ActiveSkillCommand command;
    FixedList *queue = &(combat_engine.active_commands_queue);
    while (queue->length != 0)
    {
        // Pop
        copy_buffer(fixed_list_get(queue, 0), (byte_t *)&command, queue->element_size);
        fixed_list_remove(queue, 0);

        // Execute
        command.active->metadata->execute_cb(&command);
    }
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
    }
}

void ce_broadcast_event_to_unit(CombatEventSource *source, CombatTeam *combat_team, combat_slot_t slot)
{
    CombatUnit *caster = combat_team_get_combat_unit(combat_team, slot);
    if (caster == NULL)
    {
        return;
    }

    PassiveSkillCommand command = {
        .caster = {
            .unit_id = caster->unit->id,
            .unit_slot = slot,
            .combat_team = combat_team},
        .source = source};

    SkillSet *skillset = caster->unit->skillset;
    for (size_t i = 0; i < skillset->n_passives; ++i)
    {
        PassiveSkill *skill = skillset->passives + i;
        if (skill->metadata->triggers & source->event)
        {
            command.passive = skill;
            dynamic_list_append(&(combat_engine.passive_commands_queue), (byte_t *)&command);
        }
    }
}