#include "unit.h"

void unit_initialize(Unit *unit)
{
    unit->id = UNDEFINED_UNIT_ID;
}

bool_t unit_is_valid(const Unit *unit)
{
    return unit->id != UNDEFINED_UNIT_ID;
}

bool_t unit_is_alive(const Unit *unit)
{
    return unit_is_valid(unit) && unit->hp != 0;
}

stat_t unit_calculate_stat(const Unit *unit, Stat stat)
{
    stat_t base = unit->species->base_stats[stat];
    const attribute_t *attributes = unit->attributes;
    switch (stat)
    {
    case STAT_HEALTH:
        // Calculates the maximum HP
        return base + 10 * 1 + attributes[ATTR_STRENGTH] + attributes[ATTR_RESILENCE]; // TODO: Unit's level
    case STAT_STAMINA:
        // Calculates the maximum STA
        return base + 5 * 1 + attributes[ATTR_DEXTERITY] + attributes[ATTR_AGILITY]; // TODO: Unit's level
    case STAT_DAMAGE:
        return base + 2 * attributes[ATTR_STRENGTH] + attributes[ATTR_DEXTERITY];
    case STAT_ENDURANCE:
        return base + 2 * attributes[ATTR_RESILENCE];
    case STAT_SPEED:
        return base + attributes[ATTR_DEXTERITY] + 2 * attributes[ATTR_AGILITY];
    default:
        break;
    }

    return 0;
}

bool_t unit_knows_skill(const Unit *unit, const SkillMetadata *skill_meta)
{
    // TODO
    return TRUE;
}