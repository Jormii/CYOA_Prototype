#ifndef COMBAT_DAMAGE_H
#define COMBAT_DAMAGE_H

#include "combat_descriptor.h"

typedef struct DmgCalcInstance_st
{
    CombatDescriptor *attacker;
    CombatDescriptor *defender;
} DmgCalcInstance;

void ce_damage_initialize();
void ce_damage_declare_attack(CombatDescriptor *attacker, CombatDescriptor *defender);
DmgCalcInstance *ce_damage_current();
void ce_damage_perform();

#endif