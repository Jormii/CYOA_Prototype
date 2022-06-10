#ifndef ACTIVE_SKILL_H
#define ACTIVE_SKILL_H

#include "skill.h"
#include "combat_descriptor.h"

struct ActiveSkillCommand_st;

typedef void (*ActiveSkillExecute_fp)(struct ActiveSkillCommand_st *command);

typedef struct ActiveSkillMetadata_st
{
    SkillMetadata metadata;
    ActiveSkillExecute_fp execute_cb;
} ActiveSkillMetadata;

typedef struct ActiveSkill_st
{
    const ActiveSkillMetadata *metadata;
} ActiveSkill;

typedef struct ActiveSkillCommand_st
{
    ActiveSkillMetadata *active; // TODO: ActiveSkill instead
    CombatDescriptor caster;
    CombatDescriptor target;
} ActiveSkillCommand;

#endif