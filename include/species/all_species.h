#ifndef ALL_SPECIES_H
#define ALL_SPECIES_H

#include "species.h"
#include "skillset.h"

#define ACTIVE_INIT(name) void name(ActiveSkill *skill, const ActiveSkillInfo *info)
#define ACTIVE_DEINIT(name) void name(ActiveSkill *skill)
#define ACTIVE_CAN_BE_USED(name) bool_t name(const ActiveSkill *skill, const Unit *unit)
#define ACTIVE_EXECUTE(name) stamina_t name(ActiveSkillCommand *command)

#define PASSIVE_INIT(name) void name(PassiveSkill *skill, const PassiveSkillInfo *info)
#define PASSIVE_DEINIT(name) void name(PassiveSkill *skill)
#define PASSIVE_CAN_BE_USED(name) bool_t name(const PassiveSkill *skill, const Unit *unit)
#define PASSIVE_EXECUTE(name) stamina_t name(PassiveSkillCommand *command)

ACTIVE_INIT(active_skill_init_do_nothing);
ACTIVE_DEINIT(active_skill_deinit_do_nothing);
ACTIVE_CAN_BE_USED(active_skill_can_be_used_always);

PASSIVE_INIT(passive_skill_init_do_nothing);
PASSIVE_DEINIT(passive_skill_deinit_do_nothing);
PASSIVE_CAN_BE_USED(passive_skill_can_be_used_always);

extern Species bird_species;
extern SkillSetTemplate bird_skillset_template;
extern void bird_init();

#endif