#include "ui.h"
#include "input.h"
#include "all_species.h"
#include "combat_interface.h"

State *combat_state_ask_for_action_update();
void combat_state_ask_for_action_on_enter(state_id_t previous_id);

void display_combat_state();
void display_combat_team(CombatTeam *combat_team);
void display_inventory();
void display_unit_skills(const CombatUnit *unit);
void display_skill(const SkillMetadata *metadata, size_t index);

State combat_state_ask_for_action = {
    .id = COMBAT_STATE_ASK_FOR_ACTION,
    .on_enter_cb = combat_state_ask_for_action_on_enter,
    .update_cb = combat_state_ask_for_action_update};

State *combat_state_ask_for_action_update()
{
    if (combat_interface.slot >= MAX_UNITS_IN_COMBAT)
    {
        // Actions for every unit introduced
        return &combat_state_execute;
    }

    const CombatUnit *cu = combat_team_get_combat_unit(
        &(combat_engine.players_team), combat_interface.slot);
    const SkillSetMetadata *template = cu->unit->species->skillset_template;

    // Update interface
    tb_clear(combat_interface.commands_buffer, NULL);
    if (combat_interface.showing_items)
    {
        display_inventory();
    }
    else
    {
        display_unit_skills(cu);
    }

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
        size_t limit = (combat_interface.showing_items) ? inventory.n_items : template->n_skills;
        if ((combat_interface.cursor + 1) != limit)
        {
            combat_interface.cursor += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CROSS))
    {
        combat_interface.chosen_skill = combat_interface.cursor;
        combat_interface.cursor = 0;
        return &combat_state_ask_for_action_target;
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (combat_interface.slot != 0)
        {
            combat_engine_remove_queue_tail();

            combat_interface.cursor = 0;
            combat_interface.slot -= 1;
            combat_state_ask_for_action_on_enter(-1); // TODO: Forcing this is weird
        }
    }
    else if (input_button_pressed(BUTTON_LT) || input_button_pressed(BUTTON_RT))
    {
        combat_interface.cursor = 0;
        combat_interface.showing_items = !combat_interface.showing_items;
    }

    return STATE_SAME_STATE;
}

void combat_state_ask_for_action_on_enter(state_id_t previous_id)
{
    // Update variables
    combat_interface.showing_items = FALSE;

    // Update interface
    tb_clear(combat_interface.state_buffer, NULL);
    display_combat_state();
}

void display_combat_state()
{
    tb_print(combat_interface.state_buffer, 0x00FFFFFF, L"--- YOUR UNITS ---\n");
    display_combat_team(&(combat_engine.players_team));

    tb_print(combat_interface.state_buffer, 0x00FFFFFF, L"\n--- ENEMY UNITS ---\n");
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
            if (combat_team == &(combat_engine.players_team) && slot == combat_interface.slot)
            {
                color = 0x008888FF;
            }

            const Unit *unit = combat_unit->unit;
            tb_printf(combat_interface.state_buffer, color, L"%u :: %ls (%ls / %u) :: HP: %u / %u || STA: %u / %u || (Dmg, Def, Spd): (%u, %u, %u)\n",
                      slot, unit->name, unit->species->name, unit->id,
                      unit->hp, combat_unit_calculate_stat(combat_unit, STAT_HEALTH),
                      unit->stamina, combat_unit_calculate_stat(combat_unit, STAT_STAMINA),
                      combat_unit_calculate_stat(combat_unit, STAT_DAMAGE),
                      combat_unit_calculate_stat(combat_unit, STAT_ENDURANCE),
                      combat_unit_calculate_stat(combat_unit, STAT_SPEED));
        }
    }
}

void display_inventory()
{
    tb_print(combat_interface.commands_buffer, 0x00FFFFFF, L"-- Items --\n");

    for (size_t i = 0; i < inventory.n_items; ++i)
    {
        const Item *item = inventory.items[i];
        display_skill(item->skill.metadata, i);
    }
}

void display_unit_skills(const CombatUnit *combat_unit)
{
    const Unit *unit = combat_unit->unit;
    const SkillSetMetadata *skillset_template = unit->species->skillset_template;

    tb_printf(combat_interface.commands_buffer, 0x00FFFFFF, L"-- %ls's skills --\n",
              unit->name);
    for (size_t i = 0; i < skillset_template->n_skills; ++i)
    {
        display_skill(skillset_template->skills_metadata[i], i);
    }
}

void display_skill(const SkillMetadata *metadata, size_t index)
{
    rgb_t color = 0x00FFFFFF;
    if (index == combat_interface.cursor)
    {
        color = 0x0000FFFF;
    }
    tb_printf(combat_interface.commands_buffer, color, L"%ls :: STA: %u\n",
              metadata->name, metadata->cost);
}