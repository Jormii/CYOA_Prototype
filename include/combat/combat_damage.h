#ifndef COMBAT_DAMAGE_H
#define COMBAT_DAMAGE_H

#include "combat_identifier.h"

typedef struct DmgCalcInstance_st
{
    CombatIdentifier attacker;
    CombatIdentifier defender;
} DmgCalcInstance;

void combat_damage_initialize();

void combat_damage_declare_attack(const CombatIdentifier *attacker, const CombatIdentifier *defender);
void combat_damage_perform();

DmgCalcInstance *combat_damage_peek_queue();

#endif