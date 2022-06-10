#include "all_species.h"
#include "combat_engine.h"

#include "ui.h" // TODO: Remove

#pragma region Actives

void say_hi(ActiveSkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);

    if (caster == NULL)
    {
        tb_print(&(print_window.buffer), 0x008888FF, L"(NULL) is trying to say hi\n");
    }
    else if (target == NULL)
    {
        tb_printf(&(print_window.buffer), 0x008888FF, L"%ls (%u) is trying to say hi to (NULL)\n",
                  caster->unit->name, caster->unit->id);
    }
    else
    {
        tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls (%u) says hi to %ls (%u)\n",
                  caster->unit->name, caster->unit->id, target->unit->name, target->unit->id);
    }
}

ActiveSkillMetadata active1_example = {
    .metadata = {.name = L"Say hi (1)", .cost = 1},
    .execute_cb = say_hi};

ActiveSkillMetadata active2_example = {
    .metadata = {.name = L"Say hi (2)", .cost = 2},
    .execute_cb = say_hi};

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