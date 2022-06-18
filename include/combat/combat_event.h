#ifndef COMBAT_EVENT_H
#define COMBAT_EVENT_H

#include "types.h"
#include "combat_identifier.h"

typedef enum CombatEvent_en
{
    // Engine events
    _COMBAT_EVENT_ENGINE_,
    COMBAT_EVENT_ENGINE_NONE, // TODO: Kind of ugly. Indicates that an skill is being executed
    COMBAT_EVENT_ENGINE_START_OF_ROUND,
    COMBAT_EVENT_ENGINE_END_OF_ROUND,

    // Skill events
    _COMBAT_EVENT_SKILL_,
    COMBAT_EVENT_SKILL_EXECUTION
} CombatEvent;

bool_t combat_event_is_engine_event(CombatEvent event);

typedef struct CombatEventSource_st
{
    CombatEvent event;
    size_t skill_id;
    CombatIdentifier caused_by;
} CombatEventSource;

#endif