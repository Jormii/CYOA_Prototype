#ifndef COMBAT_UNIT_H
#define COMBAT_UNIT_H

#include "unit.h"
#include "dynamic_list.h"
#include "special_condition.h"

typedef struct CombatUnit_st
{
    Unit *unit;
    bool_t died_this_round;
    SkillSet skillset;
    DynamicList special_conditions;
} CombatUnit;

void combat_unit_initialize(CombatUnit *combat_unit);

bool_t combat_unit_tag_as_dead(CombatUnit *combat_unit);
void combat_unit_apply_condition(CombatUnit *combat_unit, const SkillMetadata *metadata, const CombatIdentifier *caused_by);
void combat_unit_remove_condition(CombatUnit *combat_unit, const SkillMetadata *metadata, const CombatIdentifier *caused_by);

stat_t combat_unit_calculate_stat(const CombatUnit *combat_unit, Stat stat);

bool_t combat_unit_is_valid(const CombatUnit *combat_unit);
SpecialCondition *combat_unit_afflicted_by_condition(CombatUnit *combat_unit, const SkillMetadata *metadata, const CombatIdentifier *caused_by);

#endif