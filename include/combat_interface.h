#ifndef COMBAT_INTERFACE_H
#define COMBAT_INTERFACE_H

#include "combat_engine.h"

typedef enum CombatState_en
{
    COMBAT_STATE_START,
    COMBAT_STATE_TURN_START,
    COMBAT_STATE_ASK_SKILL,
    COMBAT_STATE_ASK_SKILL_TARGET,
    COMBAT_STATE_EXECUTE,
    COMBAT_STATE_COUNT
} CombatState;

typedef void (*CombatStateFunc_fp)();

struct
{
    // Global
    CombatState state;
    CombatStateFunc_fp state_cbs[COMBAT_STATE_COUNT];

    // General
    size_t cursor;

    // Start state
    combat_slot_t units_chosen;
} combat_interface;

void combat_interface_initialize();
void combat_interface_update();

void combat_interface_start_combat();

#endif