#ifndef SPECIAL_CONDITION_H
#define SPECIAL_CONDITION_H

#include "types.h"
#include "skill.h"
#include "combat_event.h"

struct SpecialConditionCommand_st;

typedef void (*SpecialConditionExecute_fp)(struct SpecialConditionCommand_st *command);

typedef struct SpecialConditionMetadata_st
{
    SkillMetadata metadata;
    size_t id;
    event_flags_t triggers;
    SpecialConditionExecute_fp execute_cb;
} SpecialConditionMetadata;

typedef struct SpecialCondition_st
{
    size_t cause_id; // ID of unit that inflicted the condition
    const SpecialConditionMetadata *metadata;
} SpecialCondition;

typedef struct SpecialConditionCommand_st
{
    CombatDescriptor affected;
    CombatEventSource *source;
    SpecialCondition *condition;
} SpecialConditionCommand;

#endif