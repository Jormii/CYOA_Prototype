#include "all_species.h"

#pragma region Actives

ActiveSkillMetadata active1_example = {
    .metadata = {.name = L"Active 1", .cost = 1}};

ActiveSkillMetadata active2_example = {
    .metadata = {.name = L"Active 2", .cost = 2}};

#pragma endregion

#pragma region Passives

PassiveSkillMetadata passive1_example = {
    .metadata = {.name = L"Passive 1", .cost = 3}};

PassiveSkillMetadata passive2_example = {
    .metadata = {.name = L"Passive 2", .cost = 4}};

#pragma endregion

Species bird_species = {
    .name = L"Yordana",
    .base_stats = {2, 3, 3, 1, 2},
    .skillset_template = &bird_skillset_template};

SkillSetTemplate bird_skillset_template;
ActiveSkillMetadata *actives_metadata[] = {
    &active1_example,
    &active2_example};
PassiveSkillMetadata *passives_metadata[] = {
    &passive1_example,
    &passive2_example};

void bird_init()
{
    bird_skillset_template.n_actives = 0;
    bird_skillset_template.n_passives = 0;
    bird_skillset_template.actives_metadata = actives_metadata;
    bird_skillset_template.passives_metadata = passives_metadata;

    if (actives_metadata != NULL)
    {
        bird_skillset_template.n_actives = sizeof(actives_metadata) / sizeof(ActiveSkillMetadata *);
    }
    if (passives_metadata != NULL)
    {
        bird_skillset_template.n_passives = sizeof(passives_metadata) / sizeof(PassiveSkillMetadata *);
    }
}