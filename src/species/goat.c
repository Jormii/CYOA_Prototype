#include "all_species.h"
#include "combat_damage.h"
#include "combat_engine.h"

#include "ui.h"

#if 0

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
    .execute_cb = goat_kick_execute,
    .n_passives = 0};

typedef struct GoatRamBuffer_st
{
    uint8_t consecutive_executions;
} GoatRamBuffer;

typedef struct GoatRamPassiveBuffer_st
{
    GoatRamBuffer *ptr;
} GoatRamPassiveBuffer;

void goat_ram_initialize(ActiveSkill *skill)
{
    skill->buffer = malloc(sizeof(GoatRamBuffer));
    GoatRamBuffer *buffer = (GoatRamBuffer *)skill->buffer;

    buffer->consecutive_executions = 0;

    PassiveSkill *passive = skill->passives;
    GoatRamPassiveBuffer *passive_buffer = (GoatRamPassiveBuffer *)(passive->buffer);
    passive_buffer->ptr = buffer;
}

void goat_ram_execute(ActiveSkillCommand *command)
{
    CombatUnit *caster = combat_team_get_combat_unit(command->caster.combat_team, command->caster.unit_slot);
    CombatUnit *target = combat_team_get_combat_unit(command->target.combat_team, command->target.unit_slot);
    GoatRamBuffer *buffer = (GoatRamBuffer *)(command->active->buffer);

    buffer->consecutive_executions += 1;

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls rams %ls for %u damage\n",
              caster->unit->name, target->unit->name, buffer->consecutive_executions);

    for (uint8_t i = 0; i < buffer->consecutive_executions; ++i)
    {
        ce_damage_declare_attack(&(command->caster), &(command->target));
        ce_damage_perform();
    }
}

void goat_ram_passive_initialize(PassiveSkill *skill)
{
    skill->buffer = malloc(sizeof(GoatRamPassiveBuffer));
}

void goat_ram_passive_execute(PassiveSkillCommand *command)
{
    // TODO: Check if event was triggered by this skill
}

PassiveSkillMetadata goat_ram_passive_meta = {
    .metadata = {
        .cost = 0,
        .name = L"Ram (P)",
        .priority = SKILL_PRIORITY_AVERAGE},
    .triggers = COMBAT_EVENT_SKILL_EXECUTION,
    .initialize_cb = goat_ram_passive_initialize,
    .deinitialize_cb = NULL,
    .execute_cb = goat_ram_passive_execute};

ActiveSkillMetadata goat_ram_meta = {
    .metadata = {
        .cost = 0,
        .name = L"Ram",
        .priority = SKILL_PRIORITY_AVERAGE},
    .initialize_cb = goat_ram_initialize,
    .deinitialize_cb = NULL,
    .execute_cb = goat_ram_execute,
    .n_passives = 1,
    .passives = {&goat_ram_passive_meta}};

#pragma endregion

#pragma region Passives

// TODO: Consistente

#pragma endregion

Species goat_species = {
    .name = L"Battalion",
    .base_stats = {1, 4, 1, 2, 3},
    .skillset_template = &goat_skillset_template};

SkillSetMetadata goat_skillset_template;

ActiveSkillMetadata *actives_metadata[] = {
    &goat_kick_meta,
    &goat_ram_meta};
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

#endif