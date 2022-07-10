#include "ui.h"
#include "all_skills.h"

void item_heal_execute(SkillCommand *command)
{
    CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
    if (target != NULL)
    {
        target->unit->hp += 10;

        tb_printf(&(print_window.buffer), 0x0088FF88, L"%ls heals for %u HP\n",
                  target->unit->name, 10);
    }
}

SkillMetadata item_heal_metadata = {
    .id = SKILL_ID_ITEM_HEAL_ID,
    .type = SKILL_TYPE_ACTIVE_SINGLE_ALLY,
    .cost = 0,
    .name = L"Heal",
    .description = L"Heals an ally for 10 HP",
    .priority = SKILL_PRIORITY_ITEM,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = item_heal_execute};

Item item_heal = {
    .amount = 0,
    .limit = 99,
    .skill = {
        .skill_buffer = NULL,
        .metadata = &item_heal_metadata}};