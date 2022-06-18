#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

void goat_kick_execute(SkillCommand *command)
{
    // TODO
    combat_damage_declare_attack(&(command->caster), &(command->target));
    combat_damage_perform();

    const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    const CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls kicks %ls\n",
              caster->unit->name, target->unit->name);
}

SkillMetadata goat_kick_meta = {
    .id = SKILL_ID_GOAT_KICK_ID,
    .type = SKILL_TYPE_ACTIVE_NOT_SELF,
    .cost = 0,
    .name = L"Kick",
    .description = L"Kicks a single unit, dealing damage to it",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = goat_kick_execute};