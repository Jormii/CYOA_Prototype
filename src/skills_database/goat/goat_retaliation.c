#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"
#include "unsigned_math.h"
#include "combat_interface.h"

bool_t goat_retaliation_condition_trigger(const SkillCommand *command)
{
    return FALSE;
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_END_OF_ROUND:
        return TRUE;
    case COMBAT_EVENT_UNIT_ATTACK_DECLARATION:
    {
        DmgCalcInstance *dmg_instance = combat_damage_get_instance(
            command->cause.dmg_instance_id);
        return dmg_instance->defender.unit_id == command->caster.unit_id;
    }
    default:
        break;
    }

    return FALSE;
}

extern SkillMetadata goat_retaliation_condition_meta; // Needed. TODO: Improve somehow?
void goat_retaliation_condition_execute(SkillCommand *command)
{
    return;
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_END_OF_ROUND:
    {
        CombatUnit *afflicted = combat_identifier_get_combat_unit(&(command->caster));
        combat_unit_remove_condition(
            afflicted, &goat_retaliation_condition_meta, &(command->target));
        break;
    }
    case COMBAT_EVENT_UNIT_ATTACK_DECLARATION:
    {
        DmgCalcInstance *dmg_instance = combat_damage_get_instance(
            command->cause.dmg_instance_id);
        health_t *dmg = &(dmg_instance->damage);
        *dmg = umath_substract(*dmg, 2);
        break;
    }
    default:
        break;
    }
}

SkillMetadata goat_retaliation_condition_meta = {
    .id = SKILL_ID_GOAT_RETALIATION_CONDITION_ID,
    .type = SKILL_TYPE_SPECIAL_CONDITION,
    .cost = 0,
    .name = L"Retaliation (C)",
    .description = L"This unit has increased its defense and lowered its speed",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {
        .skill_buffer_size = 0,
        .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_retaliation_condition_trigger,
    .execute_cb = goat_retaliation_condition_execute};

typedef struct GoatRetaliationBuffer_st
{
    bool_t was_attacked_this_round;
    bool_t is_second_effect;
} GoatRetaliationBuffer;

void goat_retaliation_initialize(void *skill_buffer)
{
    GoatRetaliationBuffer *buffer = (GoatRetaliationBuffer *)skill_buffer;
    buffer->was_attacked_this_round = FALSE;
    buffer->is_second_effect = FALSE;
}

bool_t goat_retaliation_trigger(const SkillCommand *command)
{
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_START_OF_ROUND:
        return TRUE;
    case COMBAT_EVENT_UNIT_ATTACK_COMPLETION:
    {
        DmgCalcInstance *dmg_instance = combat_damage_get_instance(
            command->cause.dmg_instance_id);
        return dmg_instance->defender.unit_id == command->caster.unit_id;
    }
    default:
        break;
    }
    return FALSE;
}

void goat_retaliation_execute(SkillCommand *command)
{
    CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    GoatRetaliationBuffer *buffer = (GoatRetaliationBuffer *)(command->skill->skill_buffer);
    switch (command->event)
    {
    case COMBAT_EVENT_NONE:
        if (!(buffer->is_second_effect))
        {
            tb_printf(combat_interface.state_buffer, 0x00FFFFFF,
                      L"%ls prepares itself for an incoming attack!\n",
                      caster->unit->name);

            buffer->is_second_effect = TRUE; // Update so the second effect triggers
            combat_unit_apply_condition(
                caster, &goat_retaliation_condition_meta, &(command->caster));

            // Add back this active to the queue
            SkillCommand active_command;
            combat_engine_format_active_command(
                command->skill, &(command->caster), &(command->target),
                &active_command);
            active_command.priority = SKILL_PRIORITY_LOWEST;

            combat_engine_add_command_to_queue(&active_command);
            combat_engine_execute_queue();
        }
        else
        {
            DmgCalcInstance *dmg_instance = combat_damage_declare_attack(
                &(command->caster), &(command->target), command);
            if (buffer->was_attacked_this_round)
            {
                dmg_instance->damage += 2;

                tb_printf(combat_interface.state_buffer, 0x00FFFFFF, L"%ls retaliates!\n",
                          caster->unit->name);
            }

            CombatUnit *target = combat_identifier_get_combat_unit(&(command->target));
            tb_printf(combat_interface.state_buffer, 0x00FFFFFF, L"%ls attacks %ls\n",
                      caster->unit->name, target->unit->name);

            combat_damage_perform(dmg_instance);
        }
        break;
    case COMBAT_EVENT_ENGINE_START_OF_ROUND:
        buffer->was_attacked_this_round = FALSE;
        buffer->is_second_effect = FALSE;
        break;
    case COMBAT_EVENT_UNIT_ATTACK_COMPLETION:
    {
        buffer->was_attacked_this_round = TRUE;
        break;
    }
    default:
        break;
    }
}

SkillMetadata goat_retaliation_meta = {
    .id = SKILL_ID_GOAT_RETALIATION_ID,
    .type = SKILL_TYPE_ACTIVE_SINGLE_NOT_SELF,
    .cost = 0,
    .name = L"Retaliation",
    .description = L"Attacks a unit. If this unit was attacked before that, the damage increases. Raises defense and lowers speed until the end of the round.",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {
        .skill_buffer_size = sizeof(GoatRetaliationBuffer),
        .initialize_cb = goat_retaliation_initialize},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_retaliation_trigger,
    .execute_cb = goat_retaliation_execute};