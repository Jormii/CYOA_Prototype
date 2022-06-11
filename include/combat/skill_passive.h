#ifndef PASSIVE_SKILL_H
#define PASSIVE_SKILL_H

#include "skill.h"
#include "combat_event.h"

struct PassiveSkillCommand_st;

typedef void (*PassiveSkillExecute_fp)(struct PassiveSkillCommand_st *command);

typedef struct PassiveSkillMetadata_st
{
    SkillMetadata metadata;
    event_flags_t triggers;
    PassiveSkillExecute_fp execute_cb;
} PassiveSkillMetadata;

typedef struct PassiveSkill_st
{
    const PassiveSkillMetadata *metadata;
} PassiveSkill;

typedef struct PassiveSkillCommand_st
{
    PassiveSkillMetadata *passive; // TODO: PassiveSkill instead
    CombatDescriptor caster;       // Who is casting the skill
    CombatEventSource *source;     // Who caused the event that triggered the skill
} PassiveSkillCommand;

#endif