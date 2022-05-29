#include "ui.h"
#include "input.h"
#include "skillset.h"
#include "combat_engine.h"
#include "gs_combat_engine.h"

void ask_for_skills_target_update();
void push_command();
void display_targets();
void display_target(const Unit *unit, slot_t global_slot);

void combat_engine_state_ask_for_skills_target()
{
    ask_for_skills_target_update();
    display_targets();
    display_combat_state();
}

void ask_for_skills_target_update()
{
    tb_clear(&(print_window.buffer), NULL);
    tb_clear(&(commands_window.buffer), NULL);

    // Handle input
    if (input_button_pressed(BUTTON_UP))
    {
        if (gs_ask_for_skill_buffer.target_slot != 0)
        {
            gs_ask_for_skill_buffer.target_slot -= 1;
        }
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
        if ((gs_ask_for_skill_buffer.target_slot + 1) != (2 * MAX_UNITS_IN_COMBAT))
        {
            gs_ask_for_skill_buffer.target_slot += 1;
        }
    }

    if (input_button_pressed(BUTTON_CROSS))
    {
        // Add skill to queue
        push_command();

        // Change state
        if ((gs_ask_for_skill_buffer.current + 1) < MAX_UNITS_IN_COMBAT)
        {
            gs_ask_for_skill_buffer.current += 1;
            gs_ask_for_skill_buffer.skill_index = 0;
            gs_ask_for_skill_buffer.target_slot = 0;
            gs_state_cb = combat_engine_state_ask_for_skill;
        }
        else
        {
            gs_state_cb = combat_engine_state_execute;
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        gs_state_cb = combat_engine_state_ask_for_skill;
    }
}

void push_command()
{
    // Fetch units
    Unit *caster = team_get_unit(&(combat_engine.players_team), gs_ask_for_skill_buffer.current);

    Team *target_team = &(combat_engine.enemy_team);
    if (gs_ask_for_skill_buffer.target_slot < MAX_UNITS_IN_COMBAT)
    {
        // Ally was chosen
        target_team = &(combat_engine.players_team);
    }
    slot_t target_slot = gs_ask_for_skill_buffer.target_slot % MAX_UNITS_IN_COMBAT;
    Unit *target = team_get_unit(target_team, target_slot);

    // Push to queue
    ActiveSkill *active = caster->skillset->actives + gs_ask_for_skill_buffer.skill_index;
    ActiveSkillCommand command = {
        .caster = {.unit_id = caster->id,
                   .team = &(combat_engine.players_team),
                   .slot = gs_ask_for_skill_buffer.current},
        .target = {.unit_id = target->id,
                   .team = target_team,
                   .slot = target_slot},
        .active_skill = active};
    ce_push_active_command(&command);
}

void display_targets()
{
    const Unit *unit = team_get_unit(&(combat_engine.players_team), gs_ask_for_skill_buffer.current);
    const ActiveSkill *skill = unit->skillset->actives + gs_ask_for_skill_buffer.skill_index;

    tb_printf(&(commands_window.buffer), 0xFFFFFFFF, L"-- %s --\n", skill->active_info->skill_info.name);
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        display_target(team_get_unit(&(combat_engine.players_team), slot), slot);
    }
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        display_target(team_get_unit(&(combat_engine.enemy_team), slot), slot + MAX_UNITS_IN_COMBAT);
    }
}

void display_target(const Unit *unit, slot_t global_slot)
{
    if (unit == NULL)
    {
        return;
    }

    rgb_t color = 0xFFFFFFFF;
    if (global_slot == gs_ask_for_skill_buffer.target_slot)
    {
        color = RGB(255, 255, 0);
    }
    tb_printf(&(commands_window.buffer), color, L"%s\n", unit->name);
}