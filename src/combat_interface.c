#include "ui.h"
#include "input.h"
#include "macros.h"
#include "all_species.h"
#include "combat_engine.h"
#include "combat_interface.h"

void combat_state_start();
void combat_state_turn_start();
void combat_state_ask_for_skill();
void combat_state_ask_for_skill_target();
void combat_state_execute();

void add_active_to_queue();

void display_combat_state();
void display_combat_team(CombatTeam *combat_team);
void display_unit_skills(const CombatUnit *combat_unit);
void display_skill(const SkillMetadata *metadata, size_t index);
void display_skill_targets(const SkillMetadata *metadata);
void display_skill_target(const CombatUnit *combat_unit, size_t global_slot); // TODO: Global slot is weird

// TODO: Delete this
size_t id = 0;

void init_unit(Unit *unit)
{
    unit->id = ++id;
    unit->name = L"NAME";
    unit->hp = 20;
    unit->stamina = 10;
    for (size_t i = 0; i < ATTR_COUNT; ++i)
    {
        unit->attributes[i] = 1;
    }
    unit->species = &bird_species;
}

// END TODO

void combat_interface_initialize()
{
    // Initialize combat engine and species
    ce_initialize();
    all_species_initialize();

    // Initialize interface
    combat_interface.state = COMBAT_STATE_START;
    combat_interface.state_cbs[COMBAT_STATE_START] = combat_state_start;
    combat_interface.state_cbs[COMBAT_STATE_TURN_START] = combat_state_turn_start;
    combat_interface.state_cbs[COMBAT_STATE_ASK_SKILL] = combat_state_ask_for_skill;
    combat_interface.state_cbs[COMBAT_STATE_ASK_SKILL_TARGET] = combat_state_ask_for_skill_target;
    combat_interface.state_cbs[COMBAT_STATE_EXECUTE] = combat_state_execute;

    // TODO: Remove this
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_TEAM; ++slot)
    {
        init_unit(combat_engine.players_team.team.units + slot);
        init_unit(combat_engine.enemy_team.team.units + slot);
    }
    // END TODO
}

void combat_interface_update()
{
    combat_interface.state_cbs[combat_interface.state]();
}

void combat_interface_start_combat()
{
    // Choose enemy units
    CombatTeam *combat_team = &(combat_engine.enemy_team);
    Team *team = &(combat_team->team);
    combat_slot_t limit = MIN(MAX_UNITS_IN_COMBAT, combat_team_count_available_units(combat_team));

    for (combat_slot_t slot = 0; slot < limit; ++slot)
    {
        ce_choose_unit(combat_team, team->units + slot, slot);
    }

    // Switch state
    combat_engine.in_combat = TRUE;
    combat_interface.state = COMBAT_STATE_START;
    combat_interface.cursor = 0;
    combat_interface.slot = 0;
}

void combat_state_start()
{
    // Update interface
    tb_clear(&(print_window.buffer), NULL);
    tb_print(&(print_window.buffer), 0x00FFFFFF, L"--- Choose your starting units ---\n");

    // Print all player's units
    for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_TEAM; ++slot)
    {
        rgb_t color = 0x00FFFFFF;
        if (slot == combat_interface.cursor)
        {
            color = 0x0000FFFF;
        }

        // Highlight if chosen
        const Unit *unit = team_get_unit(&(combat_engine.players_team.team), slot);
        if (unit != NULL)
        {
            if (combat_team_unit_is_in_combat(&(combat_engine.players_team), unit->id))
            {
                tb_print(&(print_window.buffer), color, L"+ ");
            }
            else
            {
                tb_print(&(print_window.buffer), color, L"- ");
            }

            tb_printf(&(print_window.buffer), color, L"%ls (%u)\n",
                      unit->name, unit->id);
        }
        else
        {
            tb_print(&(print_window.buffer), color, L"----------\n");
        }
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
        if ((combat_interface.cursor + 1) != MAX_UNITS_IN_TEAM)
        {
            combat_interface.cursor += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CROSS))
    {
        Unit *unit_highlighted = team_get_unit(&(combat_engine.players_team.team), combat_interface.cursor);
        if (unit_highlighted != NULL && !combat_team_unit_is_in_combat(&(combat_engine.players_team), unit_highlighted->id))
        {
            ce_choose_unit(&(combat_engine.players_team), unit_highlighted, combat_interface.slot);
            combat_interface.slot += 1;

            combat_slot_t available = combat_team_count_available_units(&(combat_engine.players_team));
            if (available == 0 || combat_interface.slot == MAX_UNITS_IN_COMBAT)
            {
                combat_interface.state = COMBAT_STATE_TURN_START;
                combat_interface.cursor = 0;
                combat_interface.slot = 0;
            }
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (combat_interface.slot != 0)
        {
            combat_interface.slot -= 1;
            ce_remove_from_combat(&(combat_engine.players_team), combat_interface.slot);
        }
    }
}

void combat_state_turn_start()
{
    combat_interface.cursor = 0;
    combat_interface.slot = 0;
    combat_interface.state = COMBAT_STATE_ASK_SKILL;
}

void combat_state_ask_for_skill()
{
    while (combat_interface.slot != MAX_UNITS_IN_COMBAT)
    {
        const CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.players_team), combat_interface.slot);
        if (cu == NULL)
        {
            combat_interface.slot += 1;
        }
        else
        {
            break;
        }
    }

    if (combat_interface.slot >= MAX_UNITS_IN_COMBAT)
    {
        // Force switching state to execute
        combat_interface.state = COMBAT_STATE_EXECUTE;
        return;
    }

    const CombatUnit *combat_unit = combat_team_get_combat_unit(&(combat_engine.players_team), combat_interface.slot);

    // Update interface
    tb_clear(&(print_window.buffer), NULL);
    tb_clear(&(commands_window.buffer), NULL);

    display_combat_state();
    display_unit_skills(combat_unit);

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
        const SkillSetTemplate *template = combat_unit->unit->species->skillset_template;
        size_t bound = template->n_actives + template->n_passives;
        if ((combat_interface.cursor + 1) != bound)
        {
            combat_interface.cursor += 1;
        }
    }
    else if (input_button_pressed(BUTTON_CROSS))
    {
        const SkillSetTemplate *template = combat_unit->unit->species->skillset_template;
        if (combat_interface.cursor < template->n_actives)
        {
            // Step only if an active was chosen
            combat_interface.chosen_skill = combat_interface.cursor;
            combat_interface.cursor = 0;
            combat_interface.state = COMBAT_STATE_ASK_SKILL_TARGET;
        }
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (combat_interface.slot != 0)
        {
            // TODO: Once skill queue is implemented, remove skill from queue
            combat_interface.slot -= 1;
            combat_interface.cursor = 0;
        }
    }
}

void combat_state_ask_for_skill_target()
{
    // Update interface
    tb_clear(&(print_window.buffer), NULL);
    tb_clear(&(commands_window.buffer), NULL);

    display_combat_state();

    const CombatUnit *cu = combat_team_get_combat_unit(&(combat_engine.players_team), combat_interface.slot);
    const Unit *unit = cu->unit;
    const SkillSetTemplate *skillset = unit->species->skillset_template;
    const ActiveSkillMetadata *chosen_skill = skillset->actives_metadata[combat_interface.chosen_skill];
    display_skill(&(chosen_skill->metadata), -1); // TODO: -1 is weird

    display_skill_targets(&(chosen_skill->metadata));

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
        // TODO: Check if target is valid
        add_active_to_queue();

        combat_interface.slot += 1;
        combat_interface.cursor = 0;
        combat_interface.state = COMBAT_STATE_ASK_SKILL;
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        combat_interface.cursor = combat_interface.chosen_skill;
        combat_interface.state = COMBAT_STATE_ASK_SKILL;
    }
}

void combat_state_execute()
{
    if (combat_engine.active_commands_queue.length != 0)
    {
        // Update interface
        tb_clear(&(print_window.buffer), NULL);

        // Execute queue
        ce_execute_queue();
    }
    else
    {
        // Update interface
        tb_clear(&(commands_window.buffer), NULL);
        tb_print(&(commands_window.buffer), 0x00FFFFFF, L"Executing skills... Press X to return to main state\n");

        // Handle input
        if (input_button_pressed(BUTTON_CROSS))
        {
            combat_interface.state = COMBAT_STATE_TURN_START;
        }
    }
}

void add_active_to_queue()
{
    // Caster and skill
    CombatUnit *caster = combat_team_get_combat_unit(&(combat_engine.players_team), combat_interface.slot);
    SkillSetTemplate *template = caster->unit->species->skillset_template;
    ActiveSkillMetadata *skill = template->actives_metadata[combat_interface.chosen_skill];

    // Target
    CombatTeam *targets_team = &(combat_engine.enemy_team);
    if (combat_interface.cursor < MAX_UNITS_IN_COMBAT)
    {
        // Means ally team
        targets_team = &(combat_engine.players_team);
    }

    combat_slot_t targets_slot = combat_interface.cursor % MAX_UNITS_IN_COMBAT;
    CombatUnit *target = combat_team_get_combat_unit(targets_team, targets_slot); // TODO: NULL?

    // Full command
    ActiveSkillCommand command = {.active = skill,
                                  .caster = {.unit_id = caster->unit->id, .unit_slot = combat_interface.slot, .combat_team = &(combat_engine.players_team)},
                                  .target = {.unit_id = target->unit->id, .unit_slot = targets_slot, .combat_team = targets_team}};

    // Add to queue
    ce_add_active_to_queue(&command);
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

void display_skill_targets(const SkillMetadata *metadata)
{
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