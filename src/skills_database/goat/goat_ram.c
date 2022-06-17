#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

typedef struct GoatRamBuffer_st
{
    uint8_t consecutive_uses;
} GoatRamBuffer;

void goat_ram_initialize(void *skill_buffer)
{
    GoatRamBuffer *buffer = (GoatRamBuffer *)skill_buffer;
    buffer->consecutive_uses = 0;
}

void goat_ram_execute(SkillCommand *command)
{
    Skill *skill = command->skill;
    CombatUnit *caster = combat_team_get_combat_unit(
        command->caster.combat_team, command->caster.unit_slot);
    GoatRamBuffer *buffer = (GoatRamBuffer *)(skill->skill_buffer);

    buffer->consecutive_uses += 1;
    for (uint8_t i = 0; i < buffer->consecutive_uses; ++i)
    {
        combat_damage_declare_attack(&(command->caster), &(command->target));
        combat_damage_perform();
    }

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls charges for %u damage\n",
              caster->unit->name, buffer->consecutive_uses);
}

SkillMetadata goat_ram_meta = {
    .id = SKILL_ID_GOAT_RAM_ID,
    .type = SKILL_TYPE_ACTIVE_NOT_SELF,
    .cost = 0,
    .name = L"Ram",
    .description = L"Attacks a unit. Consecutive uses increase the damage of this attack",
    .priority = SKILL_PRIORITY_AVERAGE,
    .triggers = COMBAT_EVENT_NONE,

    .initialization = {
        .skill_buffer_size = sizeof(GoatRamBuffer),
        .initialize_cb = goat_ram_initialize},
    .deinitialize_cb = NULL,
    .execute_cb = goat_ram_execute};