#ifndef ALL_SPECIES_H
#define ALL_SPECIES_H

#include "species.h"
#include "skillset.h"

void all_species_initialize();

extern Species bird_species;
extern SkillSetMetadata bird_skillset_template;
extern void bird_init();

extern Species goat_species;
extern SkillSetMetadata goat_skillset_template;
extern void goat_init();

#endif