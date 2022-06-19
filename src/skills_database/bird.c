#include <time.h>

#include "ui.h"
#include "all_species.h"
#include "combat_damage.h"
#include "combat_engine.h"
#include "unsigned_math.h"

#pragma region Actives

void say_hi(SkillCommand *command)
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

void deal_damage_initialize(void *skill_buffer)
{
    size_t upper = 5;
    size_t lower = 0;

    DealDamageBuffer *buffer = (DealDamageBuffer *)skill_buffer;
    buffer->random = (rand() % (upper - lower + 1)) + lower;
}

void deal_damage(SkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);

    DealDamageBuffer *buffer = (DealDamageBuffer *)(command->skill->skill_buffer);
    for (size_t i = 0; i < buffer->random; ++i)
    {
        combat_damage_declare_attack(&(command->caster), &(command->target));
        combat_damage_perform();
    }

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls (%u) attacks %ls (%u) for %u damage\n",
              caster->unit->name, caster->unit->id,
              target->unit->name, target->unit->id,
              buffer->random);
}

SkillMetadata bird_say_hi_meta = {
    .id = 0,
    .type = SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,
    .cost = 0,
    .name = L"Say hi!",
    .description = L"Writes to console the names of this unit and this skill's target",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = say_hi};

SkillMetadata bird_deal_random_dmg_meta = {
    .id = 1,
    .type = SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,
    .cost = 0,
    .name = L"Deal RND damage",
    .description = L"Deal a fixed amount of damage. This value is set when the unit is deployed",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = sizeof(DealDamageBuffer),
        .initialize_cb = deal_damage_initialize},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = deal_damage};

#pragma endregion

#pragma region Passives

bool_t take_damage_trigger(const SkillCommand *command)
{
    CombatEvent e = command->event;
    return e == COMBAT_EVENT_ENGINE_START_OF_ROUND || e == COMBAT_EVENT_ENGINE_END_OF_ROUND;
}

void take_damage(SkillCommand *command)
{
    CombatUnit *cu = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    cu->unit->hp = umath_substract(cu->unit->hp, 1);
}

SkillMetadata take_damage_meta = {
    .id = 2,
    .type = SKILL_TYPE_PASSIVE,
    .cost = 0,
    .name = L"Bleed",
    .description = L"Takes 1 point of damage at round start and round end",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = take_damage_trigger,
    .execute_cb = take_damage};

#pragma endregion

SkillMetadata *bird_skill[] = {&bird_say_hi_meta, &bird_deal_random_dmg_meta,
                               &take_damage_meta};
SkillSetMetadata bird_skillset_template = {
    .n_skills = sizeof(bird_skill) / sizeof(SkillMetadata *),
    .skills_metadata = bird_skill};

Species bird_species = {
    .name = L"Yordana",
    .base_stats = {2, 3, 3, 1, 2},
    .skillset_template = &bird_skillset_template};
