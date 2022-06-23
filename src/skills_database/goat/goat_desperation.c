#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

void goat_desperation_execute(SkillCommand *command)
{
    combat_damage_declare_attack(&(command->caster), &(command->target));

    DmgCalcInstance *dmg_inst = combat_damage_peek_queue();
    dmg_inst->damage = 5;
    combat_damage_perform();

    CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    const CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
    caster->unit->hp = 0;

    tb_printf(&(print_window.buffer), 0x00FFFFFF,
              L"In a desperate action, %ls sacrifices itself to damage %ls\n",
              caster->unit->name, target->unit->name);
}

SkillMetadata goat_desperation_meta = {
    .id = SKILL_ID_GOAT_DESPERATION_ID,
    .type = SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,
    .cost = 0,
    .name = L"Desperation",
    .description = L"Deals a huge amount of damage, but kills self",
    .priority = SKILL_PRIORITY_HIGH,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = goat_desperation_execute};