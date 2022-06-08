#include "all_species.h"

Species bird_species = {
    .name = L"Yordana",
    .base_stats = {2, 3, 3, 1, 2},
    .skillset_template = &bird_skillset_template};

SkillSetTemplate bird_skillset_template;

void bird_init()
{
    bird_skillset_template.n_actives = 0;
    bird_skillset_template.n_passives = 0;
}