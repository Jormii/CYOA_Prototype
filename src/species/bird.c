#include "all_species.h"
#include "combat_engine.h"

#include "ui.h" // TODO: Remove

#pragma region Actives

void say_hi(ActiveSkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls (%u) says hi to %ls (%u)\n",
              caster->unit->name, caster->unit->id, target->unit->name, target->unit->id);
}

void deal_damage(ActiveSkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);

    ce_damage_declare_attack(&(command->caster), &(command->target));
    ce_damage_perform();

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls (%u) attacks %ls (%u)\n",
              caster->unit->name, caster->unit->id, target->unit->name, target->unit->id);
}

ActiveSkillMetadata active1_example = {
    .metadata = {.name = L"Say hi", .cost = 1},
    .execute_cb = say_hi};

ActiveSkillMetadata active2_example = {
    .metadata = {.name = L"Deal damage", .cost = 2},
    .execute_cb = deal_damage};

#pragma endregion

#pragma region Passives

void take_damage(PassiveSkillCommand *command)
{
    CombatUnit *cu = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    cu->unit->hp -= 1;
}

PassiveSkillMetadata passive1_example = {
    .metadata = {.name = L"Take damage at turn start", .cost = 3},
    .triggers = COMBAT_EVENT_START_OF_TURN,
    .execute_cb = take_damage};

PassiveSkillMetadata passive2_example = {
    .metadata = {.name = L"Take damage at turn end", .cost = 4},
    .triggers = COMBAT_EVENT_END_OF_TURN,
    .execute_cb = take_damage};

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