#include "skill_includes.h"

void goat_desperation_execute(SkillCommand *command)
{
    DmgCalcInstance *dmg_instance = combat_damage_declare_attack(
        &(command->caster), &(command->target), command);

    dmg_instance->damage = 5;
    combat_damage_perform(dmg_instance);

    CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    const CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
    caster->unit->hp = 0;

    tb_printf(skills_buffer, 0x00FFFFFF,
              L"In a desperate action, %ls sacrifices itself to damage %ls\n",
              caster->unit->name, target->unit->name);
}

SkillMetadata goat_desperation_meta = {
    .id = SKILL_ID_GOAT_DESPERATION_ID,
    .type = SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,
    .attribute = ATTR_DEXTERITY,
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