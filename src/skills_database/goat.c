#include "all_species.h"

// TODO: Maybe create a huge array and simply index it?

extern SkillMetadata goat_kick_meta;
extern SkillMetadata goat_ram_meta;
extern SkillMetadata goat_unstoppable_meta;
extern SkillMetadata goat_charge_meta;

SkillMetadata *goat_skills_meta[] = {
    &goat_kick_meta,
    &goat_ram_meta,
    &goat_unstoppable_meta,
    &goat_charge_meta};

SkillSetMetadata goat_skillset_meta = {
    .n_skills = sizeof(goat_skills_meta) / sizeof(SkillMetadata *),
    .skills_metadata = goat_skills_meta};

Species goat_species = {
    .name = L"Battalion",
    .base_stats = {1, 1, 1, 1, 1},
    .skillset_template = &goat_skillset_meta};