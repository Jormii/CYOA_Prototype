#ifndef COMBAT_UNIT_H
#define COMBAT_UNIT_H

#include "unit.h"

typedef struct CombatUnit_st
{
    Unit *unit;
    bool_t died_this_round;
    SkillSet skillset;
} CombatUnit;

void combat_unit_initialize(CombatUnit *combat_unit);

bool_t combat_unit_tag_as_dead(CombatUnit *combat_unit);

bool_t combat_unit_is_valid(const CombatUnit *combat_unit);

#endif