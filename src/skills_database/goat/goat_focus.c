#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"
#include "combat_interface.h"

typedef struct GoatFocusConditionBuffer_st
{
    i8_t rounds_left;
} GoatFocusConditionBuffer;

void goat_focus_condition_initialize(void *skill_buffer)
{
    GoatFocusConditionBuffer *buffer = (GoatFocusConditionBuffer *)skill_buffer;
    buffer->rounds_left = 2;
}

bool_t goat_focus_condition_trigger(const SkillCommand *command)
{
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_END_OF_ROUND:
    case COMBAT_EVENT_CONDITION_REAPPLY:
        return TRUE;
    case COMBAT_EVENT_UNIT_ATTACK_DECLARATION:
    {
        const DmgCalcInstance *dmg_inst = combat_damage_get_instance(
            command->cause.dmg_instance_id);
        return dmg_inst->attacker.unit_id == command->caster.unit_id;
    }
    default:
        break;
    }

    return FALSE;
}

void goat_focus_condition_execute(SkillCommand *command)
{
    GoatFocusConditionBuffer *buffer = (GoatFocusConditionBuffer *)(command->skill->skill_buffer);
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_END_OF_ROUND:
        buffer->rounds_left -= 1;
        if (buffer->rounds_left == 0)
        {
            CombatUnit *affected = combat_identifier_get_combat_unit(&(command->caster));
            combat_unit_remove_condition(
                affected, command->skill->metadata, &(command->caster));
        }
        break;
    case COMBAT_EVENT_UNIT_ATTACK_DECLARATION:
    {
        DmgCalcInstance *dmg_inst = combat_damage_get_instance(command->cause.dmg_instance_id);
        dmg_inst->damage += 1;
        tb_printf(combat_interface.state_buffer, 0x00FFFFFF, L"%d\n", buffer->rounds_left);
        break;
    }
    case COMBAT_EVENT_CONDITION_REAPPLY:
        buffer->rounds_left = 2;
        break;
    default:
        break;
    }
}

SkillMetadata goat_focus_condition_meta = {
    .id = SKILL_ID_GOAT_FOCUS_CONDITION_ID,
    .type = SKILL_TYPE_SPECIAL_CONDITION,
    .cost = 0,
    .name = L"Focus (C)",
    .description = L"Increases attack and speed for two rounds",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {
        .skill_buffer_size = sizeof(GoatFocusConditionBuffer),
        .initialize_cb = goat_focus_condition_initialize},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_focus_condition_trigger,
    .execute_cb = goat_focus_condition_execute};

void goat_focus_execute(SkillCommand *command)
{
    CombatIdentifier *identifier = &(command->caster);
    CombatUnit *caster = combat_identifier_get_combat_unit(identifier);
    combat_unit_apply_condition(caster, &goat_focus_condition_meta, identifier);

    tb_printf(combat_interface.state_buffer, 0x00FFFFFF, L"%ls increases its strength and speed\n",
              caster->unit->name);
}

SkillMetadata goat_focus_meta = {
    .id = SKILL_ID_GOAT_FOCUS_ID,
    .type = SKILL_TYPE_ACTIVE_SELF,
    .cost = 0,
    .name = L"Focus",
    .description = L"Increases attack and speed for two rounds",
    .priority = SKILL_PRIORITY_AVERAGE,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = NULL,
    .execute_cb = goat_focus_execute};