#include "ui.h"
#include "input.h"
#include "combat_interface.h"

State *combat_state_ask_for_action_func();
void combat_state_ask_for_action_on_enter(state_id_t previous_id);
void combat_state_ask_for_action_on_exit(state_id_t next_id);

void display_combat_state();
void display_combat_team(CombatTeam *combat_team);
void display_unit_skills(const CombatUnit *unit);
void display_skill(const SkillMetadata *metadata, size_t index);

State combat_state_ask_for_action = {
    .id = COMBAT_STATE_ASK_FOR_ACTION,
    .func_cb = combat_state_ask_for_action_func,
    .on_enter_cb = combat_state_ask_for_action_on_enter,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_ask_for_action_func()
{
    if (combat_interface.slot >= MAX_UNITS_IN_COMBAT)
    {
        // Actions for every unit introduced
        return &combat_state_execute;
    }

    const CombatUnit *cu = combat_team_get_combat_unit(
        &(combat_engine.players_team), combat_interface.slot);
    const SkillSetTemplate *template = cu->unit->species->skillset_template;

    // Update interface
    tb_clear(&(commands_window.buffer), NULL);
    display_unit_skills(cu);

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
        size_t bound = template->n_actives + template->n_passives;
        if ((combat_interface.cursor + 1) != bound)
        {
            combat_interface.cursor += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CROSS))
    {
        if (combat_interface.cursor < template->n_actives)
        {
            // Step only if an active was chosen
            combat_interface.chosen_skill = combat_interface.cursor;
            combat_interface.cursor = 0;
            return &combat_state_ask_for_action_target;
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (combat_interface.slot != 0)
        {
            ce_remove_queue_tail();

            combat_interface.cursor = 0;
            combat_interface.slot -= 1;
            combat_state_ask_for_action_on_enter(-1); // TODO: Forcing this is weird
        }
    }

    return STATE_SAME_STATE;
}

void combat_state_ask_for_action_on_enter(state_id_t previous_id)
{
    // Update interface
    tb_clear(&(print_window.buffer), NULL);
    display_combat_state();
}

void combat_state_ask_for_action_on_exit(state_id_t next_id)
{
    if (next_id == COMBAT_STATE_ASK_FOR_ACTION_TARGET)
    {
        combat_interface.chosen_skill = combat_interface.cursor;
        combat_interface.cursor = 0;
    }
}

void display_combat_state()
{
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"--- YOUR UNITS ---\n");
    display_combat_team(&(combat_engine.players_team));

    tb_print(&(print_window.buffer), 0x00FFFFFF, L"\n--- ENEMY UNITS ---\n");
    display_combat_team(&(combat_engine.enemy_team));
}

void display_combat_team(CombatTeam *combat_team)
{
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const CombatUnit *combat_unit = combat_team_get_combat_unit(combat_team, slot);
        if (combat_unit != NULL)
        {
            rgb_t color = 0x00FFFFFF;
            if (combat_team->is_players_team && slot == combat_interface.slot)
            {
                color = 0x008888FF;
            }

            const Unit *unit = combat_unit->unit;
            tb_printf(&(print_window.buffer), color, L"%u :: %ls (%ls / %u) :: HP: %u || STA: %u\n",
                      slot, unit->name, unit->species->name, unit->id,
                      unit->hp, unit->stamina);
        }
    }
}

void display_unit_skills(const CombatUnit *combat_unit)
{
    const Unit *unit = combat_unit->unit;
    const SkillSetTemplate *skillset_template = unit->species->skillset_template;

    tb_print(&(commands_window.buffer), 0x00FFFFFF, L"-- Actives --\n");
    for (size_t i = 0; i < skillset_template->n_actives; ++i)
    {
        display_skill(&(skillset_template->actives_metadata[i]->metadata), i);
    }

    tb_print(&(commands_window.buffer), 0x00FFFFFF, L"\n-- Passives --\n");
    for (size_t i = 0; i < skillset_template->n_passives; ++i)
    {
        display_skill(&(skillset_template->passives_metadata[i]->metadata), skillset_template->n_actives + i);
    }
}

void display_skill(const SkillMetadata *metadata, size_t index)
{
    rgb_t color = 0x00FFFFFF;
    if (index == combat_interface.cursor)
    {
        color = 0x0000FFFF;
    }
    tb_printf(&(commands_window.buffer), color, L"%ls :: STA: %u\n",
              metadata->name, metadata->cost);
}