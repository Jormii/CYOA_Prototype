#include "all_species.h"
#include "combat_damage.h"

#pragma region Actives

void goat_kick_execute(ActiveSkillCommand *command)
{
    CombatDescriptor *self = &(command->caster);
    CombatDescriptor *target = &(command->target);
    ce_damage_declare_attack(self, target);
    ce_damage_perform();
};

ActiveSkillMetadata goat_kick_meta = {
    .metadata = {
        .cost = 0,
        .name = L"Kick",
        .priority = SKILL_PRIORITY_AVERAGE},
    .initialize_cb = NULL,
    .deinitialize_cb = NULL,
    .execute_cb = goat_kick_execute};

#pragma endregion

Species goat_species = {
    .name = L"Battalion",
    .base_stats = {1, 4, 1, 2, 3},
    .skillset_template = &goat_skillset_template};

SkillSetTemplate goat_skillset_template;

ActiveSkillMetadata *actives_metadata[] = {
    &goat_kick_meta};
PassiveSkillMetadata *passives_metadata = NULL;

void goat_init()
{
    goat_skillset_template.n_actives = 0;
    goat_skillset_template.n_passives = 0;
    goat_skillset_template.actives_metadata = actives_metadata;
    goat_skillset_template.passives_metadata = passives_metadata;

    if (actives_metadata != NULL)
    {
        goat_skillset_template.n_actives = sizeof(actives_metadata) / sizeof(ActiveSkillMetadata *);
    }
    if (passives_metadata != NULL)
    {
        goat_skillset_template.n_passives = sizeof(passives_metadata) / sizeof(PassiveSkillMetadata *);
    }
}