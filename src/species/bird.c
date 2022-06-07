#include "all_species.h"
#include "damage_calculator.h"

Species bird_species = {
    .id = 0,
    .name = L"Yordana",
    .base_stats = {2, 3, 3, 1, 2},
    .skillset_template = &bird_skillset_template};

SkillSetTemplate bird_skillset_template;

#pragma region Actives

ACTIVE_EXECUTE(peck_effect)
{
    damage_calculator_declare(&(command->caster), &(command->target), TRUE);
    damage_calculator_perform();

    return command->active_skill->active_info->skill_info.cost;
}

ActiveSkillInfo peck_info = {
    .skill_info = {
        .id = 0,
        .name = "Peck",
        .cost = 1,
        .priority = PRIORITY_NORMAL},
    .target = ACTIVE_TARGET_SINGLE_NOT_SELF,
    .init_cb = active_skill_init_do_nothing,
    .deinit_cb = active_skill_deinit_do_nothing,
    .can_be_used_cb = active_skill_can_be_used_always,
    .execute_cb = peck_effect};

#pragma endregion

#pragma region Passives

PASSIVE_EXECUTE(hunter_effect)
{
    DmgCalcInstance *instance = damage_calculator_get();
    if (!source_same_unit(&(command->caster), instance->attacker))
    {
        return 0;
    }

    Unit *caster = source_get_unit(&(command->caster));
    Unit *target = source_get_unit(instance->defender);
    float caster_hp_ratio = caster->hp / 100.0f;
    float target_hp_ratio = target->hp / 100.0f;
    if (caster_hp_ratio > target_hp_ratio)
    {
        instance->multiplier += 0.25f;
    }

    return command->passive_skill->passive_info->skill_info.cost;
}

PassiveSkillInfo hunter_info = {
    .skill_info = {
        .id = 1,
        .name = "Hunter (P)",
        .cost = 0,
        .priority = PRIORITY_NORMAL},
    .triggers = EVENT_CALCULATE_OFFENSE,
    .init_cb = passive_skill_init_do_nothing,
    .deinit_cb = passive_skill_deinit_do_nothing,
    .can_be_used_cb = passive_skill_can_be_used_always,
    .execute_cb = hunter_effect};

#pragma endregion

ActiveSkillInfo *bird_actives_info[] = {
    &peck_info};
PassiveSkillInfo *bird_passives_info[] = {
    &hunter_info};

void bird_init()
{
    bird_skillset_template.n_actives = sizeof(bird_actives_info) / sizeof(ActiveSkillInfo *);
    bird_skillset_template.n_passives = sizeof(bird_passives_info) / sizeof(PassiveSkillInfo *);

    bird_skillset_template.actives_info = bird_actives_info;
    bird_skillset_template.passives_info = bird_passives_info;
}