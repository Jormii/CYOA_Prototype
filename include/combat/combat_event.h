#ifndef COMBAT_EVENT_H
#define COMBAT_EVENT_H

#include "types.h"
#include "combat_identifier.h"

struct Skill_st;

typedef enum CombatEvent_en
{
    COMBAT_EVENT_NONE, // Used to indicate the execution of an active

    // Engine events
    _COMBAT_EVENT_ENGINE_,
    COMBAT_EVENT_ENGINE_START_OF_ROUND,
    COMBAT_EVENT_ENGINE_END_OF_ROUND,
    COMBAT_EVENT_ENGINE_ATTACK_DECLARATION,

    // Unit events
    _COMBAT_EVENT_UNIT_,
    COMBAT_EVENT_UNIT_DIED,

    // Skill events
    _COMBAT_EVENT_SKILL_,
    COMBAT_EVENT_SKILL_EXECUTION,

    // Condition events
    _COMBAT_EVENT_CONDITION_,
    COMBAT_EVENT_CONDITION_REAPPLY
} CombatEvent;

bool_t combat_event_is_engine_event(CombatEvent event);

#endif