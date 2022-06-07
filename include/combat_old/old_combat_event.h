#ifndef COMBAT_EVENT_H
#define COMBAT_EVENT_H

#include "types.h"
#include "unit_source.h"

typedef u32_t event_flags_t;

typedef enum CombatEvent_en
{
    EVENT_START_OF_TURN = 0x00000001,
    EVENT_BEFORE_EXECUTION = 0x00000002,
    EVENT_END_OF_TURN = 0x00000004,
    EVENT_COMBAT_DECLARATION = 0x00000008,
    EVENT_CALCULATE_OFFENSE = 0x00000010
} CombatEvent;

typedef struct CombatEventSource_st
{
    CombatEvent event;
    UnitSource responsible;
    bool_t caused_by_engine;
} CombatEventSource;

#endif