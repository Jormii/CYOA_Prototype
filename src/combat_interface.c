#include "ui.h"
#include "input.h"
#include "macros.h"
#include "all_species.h"
#include "combat_engine.h"
#include "combat_interface.h"

void on_event(const SkillCommand *event_command);

// TODO: Delete this
size_t id = 0;

void init_unit(Unit *unit, wchar_t *name, const Species *species)
{
    unit->id = ++id;
    unit->name = name;
    for (size_t i = 0; i < ATTR_COUNT; ++i)
    {
        unit->attributes[i] = 1;
    }
    unit->species = species;

    unit->hp = unit_calculate_stat(unit, STAT_HEALTH);
    unit->stamina = unit_calculate_stat(unit, STAT_STAMINA);
}

// END TODO

void combat_interface_initialize(TextBuffer *state_buffer, TextBuffer *log_buffer,
                                 TextBuffer *commands_buffer)
{
    // Initialize combat engine and species
    combat_engine_initialize();
    combat_engine.on_event_cb = on_event;

    // Hook buffers
    combat_interface.state_buffer = state_buffer;
    combat_interface.log_buffer = log_buffer;
    combat_interface.commands_buffer = commands_buffer;
    combat_interface.displaying_state = TRUE;

    // TODO: Remove this
    init_unit(combat_engine.players_team.team.units, L"First", &bird_species);
    init_unit(combat_engine.players_team.team.units + 1, L"Second", &goat_species);

    init_unit(combat_engine.enemy_team.team.units, L"Enemy 1", &bird_species);
    init_unit(combat_engine.enemy_team.team.units + 1, L"Enemy 2", &bird_species);
    // END TODO
}

State *combat_interface_update()
{
    state_manager_update(&(combat_interface.combat_state));
    if (combat_engine.in_combat)
    {
        return STATE_SAME_STATE;
    }
    else
    {
        return 0; // TODO
    }
}

void combat_interface_start_combat()
{
    state_manager_initialize(&(combat_interface.combat_state), &combat_state_start_of_combat);
}

void on_event(const SkillCommand *event_command)
{
    switch (event_command->event)
    {
    case COMBAT_EVENT_UNIT_DIED:
    {
        const CombatUnit *caster = combat_identifier_get_combat_unit(&(event_command->caster));
        const CombatUnit *target = combat_identifier_get_combat_unit(&(event_command->target));

        tb_printf(combat_interface.state_buffer, 0x00FFFFFF, L"%ls kills %ls\n", caster->unit->name, target->unit->name);
        break;
    }
    default:
        break;
    }
}