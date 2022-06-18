#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

typedef struct GoatRamBuffer_st
{
    uint8_t consecutive_uses;
} GoatRamBuffer;

void goat_ram_buffer_reset(GoatRamBuffer *buffer, SkillCommand *command)
{
    if (buffer->consecutive_uses != 0)
    {
        buffer->consecutive_uses = 0;

        CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
        tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls stops its assault\n",
                  caster->unit->name);
    }
}

void goat_ram_buffer_attack(GoatRamBuffer *buffer, SkillCommand *command)
{
    buffer->consecutive_uses += 1;

    for (uint8_t i = 0; i < buffer->consecutive_uses; ++i)
    {
        combat_damage_declare_attack(&(command->caster), &(command->target));
        combat_damage_perform();
    }

    CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls charges %ls for %u damage\n",
              caster->unit->name, target->unit->name, buffer->consecutive_uses);
}

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

    CombatEvent event = command->event;
    const SkillMetadata *cause_skill_meta = command->cause.skill->metadata;
    return event == COMBAT_EVENT_SKILL_EXECUTION && skill_metadata_is_active(cause_skill_meta) &&
           cause_skill_meta->id != SKILL_ID_GOAT_RAM_ID;
}

void goat_ram_execute(SkillCommand *command)
{
    GoatRamBuffer *buffer = (GoatRamBuffer *)(command->skill->skill_buffer);
    switch (command->event)
    {
    case COMBAT_EVENT_NONE:
        goat_ram_buffer_attack(buffer, command);
        break;
    case COMBAT_EVENT_SKILL_EXECUTION:
        goat_ram_buffer_reset(buffer, command);
        break;
    default:
        break;
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