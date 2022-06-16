#ifndef ACTIVE_SKILL_H
#define ACTIVE_SKILL_H

#include "skill.h"
#include "skill_passive.h"
#include "combat_descriptor.h"

// TODO: Unify skills

struct ActiveSkill_st;
struct ActiveSkillCommand_st;

typedef void (*ActiveSkillInitialize_fp)(struct ActiveSkill_st *skill);
typedef void (*ActiveSkillDeinitialize_fp)(struct ActiveSkill_st *skill);
typedef void (*ActiveSkillExecute_fp)(struct ActiveSkillCommand_st *command);

typedef struct ActiveSkillMetadata_st
{
    SkillMetadata metadata;
    ActiveSkillInitialize_fp initialize_cb;
    ActiveSkillDeinitialize_fp deinitialize_cb;
    ActiveSkillExecute_fp execute_cb;

    size_t n_passives;
    PassiveSkillMetadata *passives[];
} ActiveSkillMetadata;

typedef struct ActiveSkill_st
{
    const ActiveSkillMetadata *metadata;
    PassiveSkill *passives;
    void *buffer;
} ActiveSkill;

void active_skill_initialize(ActiveSkill *skill, const ActiveSkillMetadata *metadata);
void active_skill_deinitialize(ActiveSkill *skill);

typedef struct ActiveSkillCommand_st
{
    ActiveSkill *active;
    CombatDescriptor caster;
    CombatDescriptor target;
} ActiveSkillCommand;

#endif