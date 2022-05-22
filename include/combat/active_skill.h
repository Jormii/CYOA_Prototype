#ifndef ACTIVE_SKILL_H
#define ACTIVE_SKILL_H

#include "unit.h"
#include "unit_source.h"
#include "skill_common.h"

typedef enum ActiveSkillTarget_en
{
    ACTIVE_TARGET_SINGLE_NOT_SELF
} ActiveSkillTarget;

struct ActiveSkill_st;
struct ActiveSkillInfo_st;
struct ActiveSkillCommand_st;

typedef void (*ActiveSkillInit_cb)(struct ActiveSkill_st *skill,
                                   const struct ActiveSkillInfo_st *info);
typedef void (*ActiveSkillDeinit_cb)(struct ActiveSkill_st *skill);
typedef bool_t (*ActiveSkillCanBeUsed_cb)(const struct ActiveSkill_st *skill,
                                          const Unit *unit);
typedef stamina_t (*ActiveSkillExecute_cb)(struct ActiveSkillCommand_st *command);

typedef struct ActiveSkillInfo_st
{
    SkillInfo skill_info;
    ActiveSkillTarget target;
    ActiveSkillInit_cb init_cb;
    ActiveSkillDeinit_cb deinit_cb;
    ActiveSkillCanBeUsed_cb can_be_used_cb;
    ActiveSkillExecute_cb execute_cb;
} ActiveSkillInfo;

typedef struct ActiveSkill_st
{
    const ActiveSkillInfo *active_info;
    void *active_buffer;
} ActiveSkill;

typedef struct ActiveSkillCommand_st
{
    UnitSource caster;
    UnitSource target;
    ActiveSkill *active_skill;
} ActiveSkillCommand;

#endif