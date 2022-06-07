#ifndef DAMAGE_CALCULATOR_H
#define DAMAGE_CALCULATOR_H

#include "unit_source.h"

typedef struct DmgCalcInstance_st
{
    UnitSource *attacker;
    UnitSource *defender;
    float multiplier;
} DmgCalcInstance;

void damage_calculator_init();
DmgCalcInstance *damage_calculator_get();
void damage_calculator_declare(UnitSource *attacker, UnitSource *defender,
                               bool_t trigger_declaration);
void damage_calculator_perform();

#endif