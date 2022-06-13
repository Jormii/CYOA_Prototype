#include "combat_interface.h"

State *combat_state_start_of_combat_func();

State combat_state_start_of_combat = {
    .id = COMBAT_STATE_START_OF_COMBAT,
    .func_cb = combat_state_start_of_combat_func,
    .on_enter_cb = STATE_CALLBACK_NONE,
    .on_exit_cb = STATE_CALLBACK_NONE};

State *combat_state_start_of_combat_func()
{
    // If all units have been chosen, start combat formally
    combat_slot_t available = combat_team_count_available_units(&(combat_engine.players_team));
    if (available == 0 || combat_interface.slot == MAX_UNITS_IN_COMBAT)
    {
        // Choose enemy units
        CombatTeam *ct = &(combat_engine.enemy_team);
        Team *t = &(ct->team);
        for (combat_slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
        {
            Unit *unit = team_get_unit(t, slot);
            if (unit != NULL)
            {
                ce_choose_unit(ct, unit, slot);
            }
        }

        combat_engine.in_combat = TRUE;
        return &combat_state_start_of_round;
    }

    return &combat_state_choose_unit;
}