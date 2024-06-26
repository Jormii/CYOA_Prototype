#include "ui.h"
#include "input.h"
#include "all_species.h"
#include "combat_interface.h"

typedef struct TargetCbs_st
{
    void (*format_skill_command)(CombatUnit *caster, Skill *skill, SkillCommand *out_command);
    bool_t (*is_valid_target)(const Skill *skill, size_t global_slot);
    bool_t (*highlight_unit_cb)(const Skill *skill, size_t global_slot);
} TargetCbs;

#pragma region SKILL_TYPE_PASSIVE

void passive_format_skill_command(CombatUnit *caster, Skill *skill, SkillCommand *out_command)
{
}

bool_t passive_is_valid_target(const Skill *skill, size_t global_slot)
{
    return FALSE;
}

bool_t passive_highlight_unit(const Skill *skill, size_t global_slot)
{
    return FALSE;
}

TargetCbs passive_cbs = {
    .format_skill_command = passive_format_skill_command,
    .is_valid_target = passive_is_valid_target,
    .highlight_unit_cb = passive_highlight_unit};

#pragma endregion

#pragma region SKILL_TYPE_ACTIVE_SELF

void self_format_skill_command(CombatUnit *caster, Skill *skill, SkillCommand *out_command)
{
    CombatIdentifier target_identifier; // Irrelevant
    CombatIdentifier caster_identifier = {
        .unit_id = caster->unit->id,
        .unit_slot = combat_interface.slot,
        .combat_team = &(combat_engine.players_team)};

    combat_engine_format_active_command(skill, &caster_identifier, &target_identifier, out_command);
}

bool_t self_is_valid_target(const Skill *skill, size_t global_slot)
{
    return combat_interface.slot == global_slot;
}

bool_t self_highlight_unit(const Skill *skill, size_t global_slot)
{
    return combat_interface.cursor == global_slot;
}

TargetCbs self_cbs = {
    .format_skill_command = self_format_skill_command,
    .is_valid_target = self_is_valid_target,
    .highlight_unit_cb = self_highlight_unit};

#pragma endregion

#pragma region SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF

void single_not_self_format_skill_command(CombatUnit *caster, Skill *skill, SkillCommand *out_command)
{
    CombatIdentifier caster_identifier = {
        .unit_id = caster->unit->id,
        .unit_slot = combat_interface.slot,
        .combat_team = &(combat_engine.players_team)};

    CombatTeam *target_team = &(combat_engine.enemy_team);
    if (combat_interface.cursor < MAX_UNITS_IN_COMBAT)
    {
        // Means target team is player's team
        target_team = &(combat_engine.players_team);
    }

    combat_slot_t target_slot = (combat_interface.cursor) % MAX_UNITS_IN_COMBAT;
    CombatUnit *cu = combat_team_get_combat_unit(target_team, target_slot);

    CombatIdentifier target_identifier = {
        .unit_id = cu->unit->id,
        .unit_slot = target_slot,
        .combat_team = target_team};

    combat_engine_format_active_command(skill, &caster_identifier, &target_identifier, out_command);
}

bool_t single_not_self_is_valid_target(const Skill *skill, size_t global_slot)
{
    return global_slot != combat_interface.slot;
}

bool_t single_not_self_highlight_unit(const Skill *skill, size_t global_slot)
{
    return global_slot == combat_interface.cursor;
}

TargetCbs single_not_self_cbs = {
    .format_skill_command = single_not_self_format_skill_command,
    .is_valid_target = single_not_self_is_valid_target,
    .highlight_unit_cb = single_not_self_highlight_unit};

#pragma endregion

#pragma region SKILL_TYPE_ACTIVE_SINGLE_ALLY

void single_ally_format_skill_command(CombatUnit *caster, Skill *skill, SkillCommand *out_command)
{
    CombatIdentifier caster_identifier = {
        .unit_id = caster->unit->id,
        .unit_slot = combat_interface.slot,
        .combat_team = &(combat_engine.players_team)};

    combat_slot_t target_slot = (combat_interface.cursor) % MAX_UNITS_IN_COMBAT;
    CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.players_team), target_slot);
    CombatIdentifier target_identifier = {
        .unit_id = cu->unit->id,
        .unit_slot = target_slot,
        .combat_team = &(combat_engine.players_team)};

    combat_engine_format_active_command(skill, &caster_identifier, &target_identifier, out_command);
}

bool_t single_ally_is_valid_target(const Skill *skill, size_t global_slot)
{
    return global_slot < MAX_UNITS_IN_COMBAT;
}

bool_t single_ally_highlight_unit(const Skill *skill, size_t global_slot)
{
    return global_slot == combat_interface.cursor;
}

TargetCbs single_ally_cbs = {
    .format_skill_command = single_ally_format_skill_command,
    .is_valid_target = single_ally_is_valid_target,
    .highlight_unit_cb = single_ally_highlight_unit};

#pragma endregion

#pragma region SKILL_TYPE_ACTIVE_ENEMY_TEAM

void enemy_team_format_skill_command(CombatUnit *caster, Skill *skill, SkillCommand *out_command)
{
    CombatIdentifier caster_identifier = {
        .unit_id = caster->unit->id,
        .unit_slot = combat_interface.slot,
        .combat_team = &(combat_engine.players_team)};

    CombatIdentifier target_identifier = {
        .unit_id = UNDEFINED_UNIT_ID,
        .unit_slot = MAX_UNITS_IN_COMBAT,
        .combat_team = &(combat_engine.enemy_team)};

    combat_engine_format_active_command(skill, &caster_identifier, &target_identifier, out_command);
}

bool_t enemy_team_is_valid_target(const Skill *skill, size_t global_slot)
{
    return global_slot >= MAX_UNITS_IN_COMBAT;
}

bool_t enemy_team_highlight_unit(const Skill *skill, size_t global_slot)
{
    return combat_interface.cursor >= MAX_UNITS_IN_COMBAT;
}

TargetCbs enemy_team_cbs = {
    .format_skill_command = enemy_team_format_skill_command,
    .is_valid_target = enemy_team_is_valid_target,
    .highlight_unit_cb = enemy_team_highlight_unit};

#pragma endregion

State *combat_state_ask_for_action_target_update();

void add_active_to_queue(CombatUnit *caster, Skill *skill, const TargetCbs *cbs);
TargetCbs *get_skill_target_cbs(const Skill *skill);
void display_skill_targets(const Skill *skill, const TargetCbs *cbs);
void display_skill_target(const Skill *skill, const CombatUnit *combat_unit, size_t global_slot, const TargetCbs *cbs);

State combat_state_ask_for_action_target = {
    .id = COMBAT_STATE_ASK_FOR_ACTION_TARGET,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .update_cb = combat_state_ask_for_action_target_update};

State *combat_state_ask_for_action_target_update()
{
    // Update interface
    tb_clear(combat_interface.commands_buffer, NULL);

    CombatUnit *cu = combat_team_get_combat_unit(
        &(combat_engine.players_team), combat_interface.slot);
    Skill *skill = NULL;
    if (combat_interface.showing_items)
    {
        skill = &(inventory.items[combat_interface.chosen_skill]->skill);
    }
    else
    {
        skill = cu->skillset.skills + combat_interface.chosen_skill;
    }

    const TargetCbs *cbs = get_skill_target_cbs(skill);
    display_skill_targets(skill, cbs);

    // Handle input
    if (input_button_pressed(BUTTON_UP))
    {
        if (combat_interface.cursor != 0)
        {
            combat_interface.cursor -= 1;
        }
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
        if ((combat_interface.cursor + 1) != (2 * MAX_UNITS_IN_COMBAT))
        {
            combat_interface.cursor += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CROSS))
    {
        if (cbs != NULL && cbs->is_valid_target(skill, combat_interface.cursor))
        {
            add_active_to_queue(cu, skill, cbs);

            combat_interface.cursor = 0;
            combat_interface.slot += 1;
            return &combat_state_ask_for_action;
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        combat_interface.cursor = combat_interface.chosen_skill;
        return &combat_state_ask_for_action;
    }

    return STATE_SAME_STATE;
}

void add_active_to_queue(CombatUnit *caster, Skill *skill, const TargetCbs *cbs)
{
    SkillCommand command;
    cbs->format_skill_command(caster, skill, &command);
    combat_engine_add_command_to_queue(&command);
}

TargetCbs *get_skill_target_cbs(const Skill *skill)
{
    switch (skill->metadata->type)
    {
    case SKILL_TYPE_PASSIVE:
        return &passive_cbs;
    case SKILL_TYPE_ACTIVE_SELF:
        return &self_cbs;
    case SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF:
        return &single_not_self_cbs;
    case SKILL_TYPE_ACTIVE_SINGLE_ALLY:
        return &single_ally_cbs;
    case SKILL_TYPE_ACTIVE_ENEMY_TEAM:
        return &enemy_team_cbs;
    default:
        break;
    }

    return NULL;
}

void display_skill_targets(const Skill *skill, const TargetCbs *cbs)
{
    const SkillMetadata *metadata = skill->metadata;
    tb_printf(combat_interface.commands_buffer, 0x00FFFFFF, L"%ls: %ls\n",
              metadata->name, metadata->description);

    if (cbs == NULL)
    {
        tb_printf(combat_interface.commands_buffer, 0x008888FF,
                  L"Skill type %u isn't implemented yet", metadata->type);
        return;
    }

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.players_team), slot);
        display_skill_target(skill, cu, slot, cbs);
    }

    tb_print(combat_interface.commands_buffer, 0x00FFFFFF, L"\n");

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.enemy_team), slot);
        display_skill_target(skill, cu, slot + MAX_UNITS_IN_COMBAT, cbs);
    }
}

void display_skill_target(const Skill *skill, const CombatUnit *combat_unit, size_t global_slot, const TargetCbs *cbs)
{
    if (combat_unit == NULL)
    {
        tb_print(combat_interface.commands_buffer, 0x00888888, L"- ----------\n");
    }
    else
    {
        rgb_t color = 0x00FFFFFF;
        bool_t highlight = cbs->highlight_unit_cb(skill, global_slot);
        bool_t valid = cbs->is_valid_target(skill, global_slot);
        if (highlight && valid)
        {
            color = 0x0000FFFF;
        }
        else if (highlight)
        {
            color = 0x008888FF;
        }
        else if (!valid)
        {
            color = 0x00888888;
        }

        const Unit *unit = combat_unit->unit;
        tb_printf(combat_interface.commands_buffer, color, L"- %ls (%ls / %u)\n",
                  unit->name, unit->species->name, unit->id);
    }
}