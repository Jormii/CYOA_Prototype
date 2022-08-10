#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"
#include "combat_interface.h"

void goat_kick_execute(SkillCommand *command)
{
    // TODO
    DmgCalcInstance *dmg_instance = combat_damage_declare_attack(
        &(command->caster), &(command->target), command);
    combat_damage_perform(dmg_instance);

    const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    const CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
    tb_printf(combat_interface.state_buffer, 0x00FFFFFF, L"%ls kicks %ls for %u damage\n",
              caster->unit->name, target->unit->name, dmg_instance->damage_dealt);
}

SkillMetadata goat_kick_meta = {
    .id = SKILL_ID_GOAT_KICK_ID,
    .type = SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,
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