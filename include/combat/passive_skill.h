#ifndef PASSIVE_SKILL_H
#define PASSIVE_SKILL_H

#include "unit.h"
#include "unit_source.h"
#include "combat_event.h"
#include "skill_common.h"

struct PassiveSkill_st;
struct PassiveSkillInfo_st;
struct PassiveSkillCommand_st;

typedef void (*PassiveSkillInit_fp)(struct PassiveSkill_st *skills,
                                    const struct PassiveSkillInfo_st *info);
typedef void (*PassiveSkillDeinit_fp)(struct PassiveSkill_st *skill);
typedef bool_t (*PassiveSkillCanBeUsed_fp)(const struct PassiveSkill_st *skill,
                                           const Unit *unit);
typedef stamina_t (*PassiveSkillExecute_fp)(struct PassiveSkillCommand_st *command);

typedef struct PassiveSkillInfo_st
{
    SkillInfo skill_info;
    event_flags_t triggers;
    PassiveSkillInit_fp init_cb;
    PassiveSkillDeinit_fp deinit_cb;
    PassiveSkillCanBeUsed_fp can_be_used_cb;
    PassiveSkillExecute_fp execute_cb;
} PassiveSkillInfo;

typedef struct PassiveSkill_st
{
    const PassiveSkillInfo *passive_info;
    void *passive_buffer;
} PassiveSkill;

typedef struct PassiveSkillCommand_st
{
    UnitSource caster;
    CombatEventSource *source;
    PassiveSkill *passive_skill;
} PassiveSkillCommand;

#endif