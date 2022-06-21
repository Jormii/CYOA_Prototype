#ifndef SKILL_H
#define SKILL_H

#include "types.h"
#include "combat_event.h"
#include "combat_identifier.h"

struct SkillCommand_st;
typedef void (*SkillInitialize_fp)(void *skill_buffer);
typedef void (*SkillDeinitialize_fp)(void *skill_buffer);
typedef bool_t (*SkillTrigger_fp)(const struct SkillCommand_st *command);
typedef void (*SkillExecute_fp)(struct SkillCommand_st *command);

typedef enum SkillType_en
{
    SKILL_TYPE_PASSIVE,
    SKILL_TYPE_SPECIAL_CONDITION,
    _SKILL_TYPE_ACTIVE_,

    // Single-targeted
    SKILL_TYPE_ACTIVE_SELF,
    SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,

    // Team-targeted
    SKILL_TYPE_ACTIVE_ENEMY_TEAM,
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

    SkillInitialization initialization;
    SkillDeinitialize_fp deinitialize_cb;
    SkillTrigger_fp trigger_cb;
    SkillExecute_fp execute_cb;
} SkillMetadata;

bool_t skill_metadata_is_active(const SkillMetadata *metadata);
bool_t skill_metadata_targets_single_unit(const SkillMetadata *metadata);

typedef struct Skill_st
{
    const SkillMetadata *metadata;
    void *skill_buffer;
} Skill;

void skill_initialize(Skill *skill, const SkillMetadata *metadata);
void skill_deinitialize(Skill *skill);

typedef struct SkillCommandCause_st
{
    Skill *skill;
    CombatEvent event;
    CombatIdentifier caster;
} SkillCommandCause;

bool_t skill_command_cause_is_active(const SkillCommandCause *cause);

typedef struct SkillCommand_st
{
    Skill *skill;
    CombatEvent event;
    CombatIdentifier caster;
    CombatIdentifier target;
    SkillCommandCause cause;

    bool_t broadcasted;
    bool_t executed;
} SkillCommand;

bool_t skill_command_is_active(const SkillCommand *command);
bool_t skill_command_caster_is_cause_of_event(const SkillCommand *command);

#endif