#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

void goat_charge_execute(SkillCommand *command)
{
    // TODO: Make it truly random
    combat_slot_t target_slot = 0;
    CombatUnit *target_unit = NULL;
    CombatTeam *target_team = command->target.combat_team;
    for (; target_slot < MAX_UNITS_IN_COMBAT; ++target_slot)
    {
        target_unit = combat_team_get_combat_unit(target_team, target_slot);
        if (target_unit != NULL && unit_is_alive(target_unit->unit))
        {
            break;
        }
        else
        {
            target_unit = NULL;
        }
    }

    if (target_unit != NULL)
    {
        CombatIdentifier target_identifier = {
            .unit_id = target_unit->unit->id,
            .unit_slot = target_slot,
            .combat_team = target_team};

        DmgCalcInstance *dmg_instance = combat_damage_declare_attack(
            &(command->caster), &target_identifier, command);
        combat_damage_perform(dmg_instance);

        const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
        tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls charges, hitting %ls\n",
                  caster->unit->name, target_unit->unit->name);
    }
}

SkillMetadata goat_charge_meta = {
    .id = SKILL_ID_GOAT_CHARGE_ID,
    .type = SKILL_TYPE_ACTIVE_ENEMY_TEAM,
    .cost = 0,
    .name = L"Charge!",
    .description = L"Deals a lot of damage to a random enemy unit",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = goat_charge_execute};