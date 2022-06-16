#ifndef SKILL_H
#define SKILL_H

#include "types.h"
#include "combat_event.h"
#include "combat_descriptor.h"

struct SkillCommand_st;
typedef void (*SkillInitialize_fp)(void *skill_buffer);
typedef void (*SkillDeinitialize_fp)(void *skill_buffer);
typedef void (*SkillExecute_fp)(struct SkillCommand_st *command);

typedef enum SkillType_en
{
    SKILL_TYPE_PASSIVE,
    SKILL_TYPE_SPECIAL_CONDITION,
    SKILL_TYPE_ACTIVE_NOT_SELF
} SkillType;

typedef enum SkillPriority_en
{
    SKILL_PRIORITY_LOWEST,
    SKILL_PRIORITY_LOW,
    SKILL_PRIORITY_AVERAGE,
    SKILL_PRIORITY_HIGH,
    SKILL_PRIORITY_HIGHEST
} SkillPriority;

typedef struct SkillInitialization_st
{
    size_t skill_buffer_size;
    SkillInitialize_fp initialize_cb;
} SkillInitialization;

typedef struct SkillMetadata_st
{
    size_t id;
    SkillType type;
    stamina_t cost;
    const wchar_t *name;
    const wchar_t *description;
    SkillPriority priority;
    event_flags_t triggers;

    SkillInitialization initialization;
    SkillDeinitialize_fp deinitialize_cb;
    SkillExecute_fp execute_cb;
} SkillMetadata;

typedef struct Skill_st
{
    const SkillMetadata *metadata;
    void *skill_buffer;
} Skill;

void skill_initialize(Skill *skill, const SkillMetadata *metadata);
void skill_deinitialize(Skill *skill);

typedef struct SkillCommand_st
{
    Skill *skill;
    CombatDescriptor caster;
    CombatDescriptor target;
    CombatEventSource event_source;
} SkillCommand;

#endif