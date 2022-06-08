#ifndef SPECIES_H
#define SPECIES_H

#include "stat.h"
#include "types.h"
#include "skillset.h"

typedef struct Species_st
{
    const wchar_t *name;
    stat_t base_stats[STAT_COUNT];
    SkillSetTemplate *skillset_template;
} Species;

#endif