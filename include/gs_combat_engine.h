#ifndef GS_COMBAT_ENGINE_H
#define GS_COMBAT_ENGINE_H

#include "team.h"

typedef void (*CombatEngineState_fp)();

CombatEngineState_fp gs_state_cb;

struct
{
    slot_t current;
    size_t skill_index;
    slot_t target_slot; // [0-N): Player team, [N, 2N): Enemy team
} gs_ask_for_skill_buffer;

void gs_combat_engine_start();
void combat_engine_state_start_of_turn();
void combat_engine_state_ask_for_skill();
void combat_engine_state_ask_for_skills_target();
void combat_engine_state_execute();

void display_skills();
void display_combat_state();

#endif