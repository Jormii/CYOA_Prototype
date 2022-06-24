#ifndef COMBAT_DAMAGE_H
#define COMBAT_DAMAGE_H

#include "types.h"
#include "skill.h"
#include "combat_identifier.h"

typedef struct DmgCalcInstance_st
{
    size_t id;
    health_t damage;
    SkillCommand *cause;
    CombatIdentifier attacker;
    CombatIdentifier defender;
} DmgCalcInstance;

void combat_damage_initialize();

DmgCalcInstance *combat_damage_declare_attack(const CombatIdentifier *attacker, const CombatIdentifier *defender, SkillCommand *cause);
void combat_damage_perform(DmgCalcInstance *dmg_instance);
void combat_damage_reset();

DmgCalcInstance *combat_damage_get_instance(size_t dmg_instance_id);

#endif