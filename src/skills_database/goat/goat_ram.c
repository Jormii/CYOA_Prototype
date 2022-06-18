#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

typedef struct GoatRamBuffer_st
{
    uint8_t consecutive_uses;
    bool_t apply_passive;
} GoatRamBuffer;

void goat_ram_initialize(void *skill_buffer)
{
    GoatRamBuffer *buffer = (GoatRamBuffer *)skill_buffer;
    buffer->consecutive_uses = 0;
}

bool_t goat_ram_trigger(const SkillCommand *command)
{
    if (!skill_command_caster_is_cause_of_event(command))
    {
        return FALSE;
    }

    CombatEvent event = command->cause.event;
    size_t skill_id = command->cause.skill_id;
    return event == COMBAT_EVENT_SKILL_EXECUTION && skill_id != SKILL_ID_GOAT_RAM_ID;
}

void goat_ram_attack(SkillCommand *command)
{
    CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    GoatRamBuffer *buffer = (GoatRamBuffer *)(command->skill->skill_buffer);
    buffer->consecutive_uses += 1;

    for (uint8_t i = 0; i < buffer->consecutive_uses; ++i)
    {
        combat_damage_declare_attack(&(command->caster), &(command->target));
        combat_damage_perform();
    }

    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls charges for %u damage\n",
              caster->unit->name, buffer->consecutive_uses);
}

void goat_ram_reset(SkillCommand *command)
{
    CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    GoatRamBuffer *buffer = (GoatRamBuffer *)(command->skill->skill_buffer);
    if (buffer->consecutive_uses != 0)
    {
        buffer->consecutive_uses = 0;

        tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls stops its assault\n",
                  caster->unit->name);
    }
}

void goat_ram_execute(SkillCommand *command)
{
    CombatEvent event = command->cause.event;
    if (event == COMBAT_EVENT_ENGINE_NONE)
    {
        goat_ram_attack(command);
    }
    else
    {
        goat_ram_reset(command);
    }
}

SkillMetadata goat_ram_meta = {
    .id = SKILL_ID_GOAT_RAM_ID,
    .type = SKILL_TYPE_ACTIVE_NOT_SELF,
    .cost = 0,
    .name = L"Ram",
    .description = L"Attacks a unit. Consecutive uses increase the damage of this attack",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = sizeof(GoatRamBuffer),
        .initialize_cb = goat_ram_initialize},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_ram_trigger,
    .execute_cb = goat_ram_execute};