#ifndef COMBAT_EVENT_H
#define COMBAT_EVENT_H

#include "types.h"
#include "combat_descriptor.h"

typedef enum CombatEvent_en
{
    COMBAT_EVENT_START_OF_TURN = 0x00000001,
    COMBAT_EVENT_END_OF_TURN = 0x00000002
} CombatEvent;

typedef struct CombatEventSource_st
{
    CombatEvent event;
    CombatDescriptor caused_by;
    bool_t caused_by_engine;
} CombatEventSource;

#endif