#include "ui.h"
#include "input.h"
#include "macros.h"
#include "all_species.h"
#include "combat_engine.h"
#include "combat_interface.h"

// TODO: Delete this
size_t id = 0;

void init_unit(Unit *unit, wchar_t *name, const Species *species)
{
    unit->id = ++id;
    unit->name = name;
    unit->hp = 20;
    unit->stamina = 10;
    for (size_t i = 0; i < ATTR_COUNT; ++i)
    {
        unit->attributes[i] = 1;
    }
    unit->attributes[ATTR_AGILITY] = unit->id + 1;
    unit->species = species;
}

// END TODO

void combat_interface_initialize()
{
    // Initialize combat engine and species
    combat_engine_initialize();

    // TODO: Remove this
    init_unit(combat_engine.players_team.team.units, L"First", &bird_species);
    init_unit(combat_engine.players_team.team.units + 1, L"Second", &bird_species);

    init_unit(combat_engine.enemy_team.team.units, L"Enemy 1", &bird_species);
    init_unit(combat_engine.enemy_team.team.units + 1, L"Enemy 2", &bird_species);
    // END TODO
}

State *combat_interface_update()
{
    state_manager_execute(&(combat_interface.combat_state));
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
    combat_interface.combat_state.state = &combat_state_start_of_combat;
}