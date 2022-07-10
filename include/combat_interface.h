#ifndef COMBAT_INTERFACE_H
#define COMBAT_INTERFACE_H

#include "state.h"
#include "combat_engine.h"

extern State combat_state_start_of_combat;
extern State combat_state_start_of_round;
extern State combat_state_ask_for_action;
extern State combat_state_ask_for_action_target;
extern State combat_state_check_units;
extern State combat_state_choose_unit;
extern State combat_state_execute;
extern State combat_state_end_of_combat;

typedef enum CombatStateIdentifier_en
{
    COMBAT_STATE_START_OF_COMBAT,
    COMBAT_STATE_START_OF_ROUND,
    COMBAT_STATE_ASK_FOR_ACTION,
    COMBAT_STATE_ASK_FOR_ACTION_TARGET,
    COMBAT_STATE_CHECK_UNITS,
    COMBAT_STATE_CHOOSE_UNIT,
    COMBAT_STATE_EXECUTE,
    COMBAT_STATE_END_OF_COMBAT
} CombatStateIdentifier;

struct
{
    StateManager combat_state;

    size_t cursor;
    combat_slot_t slot;
    size_t chosen_skill;
    bool_t showing_items;
} combat_interface;

void combat_interface_initialize();
State *combat_interface_update();

void combat_interface_start_combat();

#endif