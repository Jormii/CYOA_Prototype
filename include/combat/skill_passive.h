#ifndef PASSIVE_SKILL_H
#define PASSIVE_SKILL_H

#include "skill.h"
#include "combat_event.h"

struct PassiveSkill_st;
struct PassiveSkillCommand_st;

typedef void (*PassiveSkillInitialize_fp)(struct PassiveSkill_st *skill);
typedef void (*PassiveSkillDeinitialize_fp)(struct PassiveSkill_st *skill);
typedef void (*PassiveSkillExecute_fp)(struct PassiveSkillCommand_st *command);

typedef struct PassiveSkillMetadata_st
{
    SkillMetadata metadata;
    event_flags_t triggers;
    PassiveSkillInitialize_fp initialize_cb;
    PassiveSkillDeinitialize_fp deinitialize_cb;
    PassiveSkillExecute_fp execute_cb;
} PassiveSkillMetadata;

typedef struct PassiveSkill_st
{
    const PassiveSkillMetadata *metadata;
    void *buffer;
} PassiveSkill;

void passive_skill_initialize(PassiveSkill *skill, const PassiveSkillMetadata *metadata);
void passive_skill_deinitialize(PassiveSkill *skill);

typedef struct PassiveSkillCommand_st
{
    PassiveSkill *passive;
    CombatDescriptor caster;   // Who is casting the skill
    CombatEventSource *source; // Who caused the event that triggered the skill
} PassiveSkillCommand;

#endif