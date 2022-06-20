#include "combat_event.h"

bool_t combat_event_is_engine_event(CombatEvent event)
{
    return event > _COMBAT_EVENT_ENGINE_ && event < _COMBAT_EVENT_SKILL_;
}