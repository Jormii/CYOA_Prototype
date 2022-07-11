#include "all_species.h"

// TODO: Maybe create a huge array and simply index it?

extern SkillMetadata goat_kick_meta;
extern SkillMetadata goat_ram_meta;
extern SkillMetadata goat_unstoppable_meta;
extern SkillMetadata goat_focus_meta;
extern SkillMetadata goat_charge_meta;
extern SkillMetadata goat_desperation_meta;
extern SkillMetadata goat_penetration_meta;
extern SkillMetadata goat_retaliation_meta;
extern SkillMetadata goat_anticipation_meta;

SkillMetadata *goat_skills_meta[] = {
    &goat_kick_meta,
    &goat_ram_meta,
    &goat_unstoppable_meta,
    &goat_focus_meta,
    &goat_charge_meta,
    &goat_desperation_meta,
    &goat_penetration_meta,
    &goat_retaliation_meta,
    &goat_anticipation_meta};

SkillSetMetadata goat_skillset_meta = {
    .n_skills = sizeof(goat_skills_meta) / sizeof(SkillMetadata *),
    .skills_metadata = goat_skills_meta};

Species goat_species = {
    .name = L"Battalion",
    .base_stats = {1, 2, 2, 3, 3},
    .skillset_template = &goat_skillset_meta};