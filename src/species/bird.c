#include <time.h>

#include "all_species.h"
#include "combat_engine.h"
#include "unsigned_math.h"
#include "special_condition.h"

#include "ui.h" // TODO: Remove

#pragma region Conditions

void deplete_stamina_condition(SpecialConditionCommand *command)
{
    CombatUnit *cu = combat_team_get_combat_unit(
        command->affected.combat_team, command->affected.unit_slot);
    cu->unit->stamina = 0;

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls's STA has been set to 0\n",
              cu->unit->name);
}

SpecialConditionMetadata deplete_stamina_condition_meta = {
    .id = 0,
    .metadata = {
        .cost = 0,
        .name = L"Deplete STA at round end",
        .priority = SKILL_PRIORITY_AVERAGE},
    .triggers = COMBAT_EVENT_END_OF_TURN,
    .execute_cb = deplete_stamina_condition};

#pragma endregion

#pragma region Actives

void say_hi(ActiveSkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls (%u) says hi to %ls (%u)\n",
              caster->unit->name, caster->unit->id, target->unit->name, target->unit->id);
}

typedef struct DealDamageBuffer_st
{
    size_t random;
} DealDamageBuffer;

void deal_damage_initialize(ActiveSkill *skill)
{
    size_t upper = 5;
    size_t lower = 0;

    skill->buffer = malloc(sizeof(DealDamageBuffer));
    DealDamageBuffer *buffer = (DealDamageBuffer *)(skill->buffer);

    buffer->random = (rand() % (upper - lower + 1)) + lower;
}

void deal_damage(ActiveSkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);

    DealDamageBuffer *buffer = (DealDamageBuffer *)(command->active->buffer);
    for (size_t i = 0; i < buffer->random; ++i)
    {
        ce_damage_declare_attack(&(command->caster), &(command->target));
        ce_damage_perform();
    }

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls (%u) attacks %ls (%u) for %u damage\n",
              caster->unit->name, caster->unit->id,
              target->unit->name, target->unit->id,
              buffer->random);
}

void deplete_stamina(ActiveSkillCommand *command)
{
    CombatUnit *target = combat_team_get_combat_unit(
        command->target.combat_team, command->target.unit_slot);
    ce_apply_condition(command->caster.unit_id, target, &deplete_stamina_condition_meta);
}

ActiveSkillMetadata active1_example = {
    .metadata = {.name = L"Say hi", .cost = 1, .priority = SKILL_PRIORITY_AVERAGE},
    .execute_cb = say_hi};

ActiveSkillMetadata active2_example = {
    .metadata = {.name = L"Deal damage equal to random number", .cost = 2, .priority = SKILL_PRIORITY_AVERAGE},
    .initialize_cb = deal_damage_initialize,
    .execute_cb = deal_damage};

ActiveSkillMetadata active3_example = {
    .metadata = {.name = L"Deplete target STA at round end", .cost = 0, .priority = SKILL_PRIORITY_AVERAGE},
    .execute_cb = deplete_stamina};

#pragma endregion

#pragma region Passives

void take_damage(PassiveSkillCommand *command)
{
    CombatUnit *cu = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    cu->unit->hp = umath_substract(cu->unit->hp, 1);
}

PassiveSkillMetadata passive1_example = {
    .metadata = {.name = L"Take damage at start of turn", .cost = 3, .priority = SKILL_PRIORITY_AVERAGE},
    .triggers = COMBAT_EVENT_START_OF_TURN,
    .execute_cb = take_damage};

PassiveSkillMetadata passive2_example = {
    .metadata = {.name = L"Reduce STA to 0 at", .cost = 4, .priority = SKILL_PRIORITY_AVERAGE},
    .triggers = COMBAT_EVENT_END_OF_TURN,
    .execute_cb = take_damage};

#pragma endregion

Species bird_species = {
    .name = L"Yordana",
    .base_stats = {2, 3, 3, 1, 2},
    .skillset_template = &bird_skillset_template};

SkillSetTemplate bird_skillset_template;
ActiveSkillMetadata *bird_actives_metadata[] = {
    &active1_example,
    &active2_example,
    &active3_example};
PassiveSkillMetadata *bird_passives_metadata[] = {
    &passive1_example,
    &passive2_example};

void bird_init()
{
    srand(time(NULL)); // TODO: Remove
    bird_skillset_template.n_actives = 0;
    bird_skillset_template.n_passives = 0;
    bird_skillset_template.actives_metadata = bird_actives_metadata;
    bird_skillset_template.passives_metadata = bird_passives_metadata;

    if (bird_actives_metadata != NULL)
    {
        bird_skillset_template.n_actives = sizeof(bird_actives_metadata) / sizeof(ActiveSkillMetadata *);
    }
    if (bird_passives_metadata != NULL)
    {
        bird_skillset_template.n_passives = sizeof(bird_passives_metadata) / sizeof(PassiveSkillMetadata *);
    }
}