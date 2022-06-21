#ifndef SPECIAL_CONDITION_H
#define SPECIAL_CONDITION_H

#include "skill.h"
#include "combat_identifier.h"

typedef struct SpecialCondition_st
{
    Skill skill;
    CombatIdentifier caused_by;
} SpecialCondition;

void special_condition_reapply(SpecialCondition *condition, const CombatIdentifier *afflicted);

bool_t special_condition_are_equivalent(const SpecialCondition *condition, const SkillMetadata *metadata, const CombatIdentifier *caused_by);

#endif
