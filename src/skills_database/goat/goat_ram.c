#include "ui.h"
#include "all_skills.h"

typedef struct GoatRamBuffer_st
{
    uint8_t consecutive_uses;
} GoatRamBuffer;

void goat_ram_initialize(void *skill_buffer)
{
    GoatRamBuffer *buffer = (GoatRamBuffer *)skill_buffer;
    buffer->consecutive_uses = 0;
}

bool_t goat_ram_trigger(const SkillCommand *command)
{
    const CombatEventSource *source = &(command->event_source);
    if (source->cause != COMBAT_EVENT_CAUSE_SKILL)
    {
        return FALSE;
    }

    bool_t same_unit = command->caster.unit_id == command->event_source.caused_by.unit_id;
    bool_t correct_event = command->event_source.event == COMBAT_EVENT_ACTIVE_EXECUTION;

    return same_unit && correct_event;
}

void goat_ram_execute(SkillCommand *command)
{
    Skill *skill = command->skill;
    CombatUnit *caster = combat_team_get_combat_unit(
        command->caster.combat_team, command->caster.unit_slot);
    GoatRamBuffer *buffer = (GoatRamBuffer *)(skill->skill_buffer);
    if (skill->metadata->id != SKILL_ID_GOAT_RAM_ID)
    {
        if (buffer->consecutive_uses != 0)
        {
            buffer->consecutive_uses = 0;

            tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls stops ramming its opponents\n",
                      caster->unit->name);
        }
        return;
    }

    buffer->consecutive_uses += 1;
    for (uint8_t i = 0; i < buffer->consecutive_uses; ++i)
    {
        ce_damage_declare_attack(&(command->caster), &(command->target));
        ce_damage_perform();
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

    .initialization = {
        .skill_buffer_size = sizeof(GoatRamBuffer),
        .initialize_cb = goat_ram_initialize},
    .deinitialize_cb = NULL,
    .trigger_fp = goat_ram_trigger,
    .execute_cb = goat_ram_execute};