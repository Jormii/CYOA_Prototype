#include "ui.h"
#include "input.h"
#include "combat_interface.h"

State *combat_state_ask_for_action_target_func();

void add_active_to_queue();
void display_skill_targets(const SkillMetadata *metadata);
void display_skill_target(const CombatUnit *combat_unit, size_t global_slot); // TODO: Global slot is weird

State combat_state_ask_for_action_target = {
    .id = COMBAT_STATE_ASK_FOR_ACTION_TARGET,
    .func_cb = combat_state_ask_for_action_target_func,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_ask_for_action_target_func()
{
    // Update interface
    tb_clear(&(commands_window.buffer), NULL);

    const CombatUnit *cu = combat_team_get_combat_unit(
        &(combat_engine.players_team), combat_interface.slot);
    const SkillSetMetadata *template = cu->unit->species->skillset_template;

    display_skill_targets(template->skills_metadata[combat_interface.chosen_skill]);

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
        add_active_to_queue(); // TODO: Check if target is valid

        combat_interface.cursor = 0;
        combat_interface.slot += 1;
        return &combat_state_ask_for_action;
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        combat_interface.cursor = combat_interface.chosen_skill;
        return &combat_state_ask_for_action;
    }

    return STATE_SAME_STATE;
}

void add_active_to_queue()
{
    // Caster and skill
    CombatUnit *caster = combat_team_get_combat_unit(&(combat_engine.players_team), combat_interface.slot);
    SkillSet *skillset = &(caster->skillset);
    Skill *skill = skillset->skills + combat_interface.chosen_skill;

    // Target
    CombatTeam *targets_team = &(combat_engine.enemy_team);
    if (combat_interface.cursor < MAX_UNITS_IN_COMBAT)
    {
        // Means ally team
        targets_team = &(combat_engine.players_team);
    }

    combat_slot_t targets_slot = combat_interface.cursor % MAX_UNITS_IN_COMBAT;
    CombatUnit *target = combat_team_get_combat_unit(targets_team, targets_slot); // TODO: What if NULL?

    // Add to queue
    CombatIdentifier caster_identifier = {
        .unit_id = caster->unit->id,
        .unit_slot = combat_interface.slot,
        .combat_team = &(combat_engine.players_team)};
    CombatIdentifier target_identifier = {
        .unit_id = target->unit->id,
        .unit_slot = targets_slot,
        .combat_team = targets_team};

    combat_engine_add_active_to_queue(skill, &caster_identifier, &target_identifier);
}

void display_skill_targets(const SkillMetadata *metadata)
{
    tb_printf(&(commands_window.buffer), 0x00FFFFFF, L"%ls: %ls\n",
              metadata->name, metadata->description);

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.players_team), slot);
        display_skill_target(cu, slot);
    }

    tb_print(&(commands_window.buffer), 0x00FFFFFF, L"\n");

    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.enemy_team), slot);
        display_skill_target(cu, slot + MAX_UNITS_IN_COMBAT);
    }
}

void display_skill_target(const CombatUnit *combat_unit, size_t global_slot)
{
    rgb_t color = 0x00FFFFFF;
    if (combat_interface.cursor == global_slot)
    {
        color = 0x0000FFFF;
    }

    if (combat_unit == NULL)
    {
        tb_print(&(commands_window.buffer), color, L"- ----------\n");
    }
    else
    {
        const Unit *unit = combat_unit->unit;
        tb_printf(&(commands_window.buffer), color, L"- %ls (%ls / %u)\n",
                  unit->name, unit->species->name, unit->id);
    }
}