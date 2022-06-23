#include "ui.h"
#include "all_skills.h"
#include "combat_damage.h"

typedef struct GoatPenetrationConditionBuffer_st
{
    u8_t stacks;
    u8_t rounds_left;
} GoatPenetrationConditionBuffer;

void goat_penetration_condition_initialize(void *skill_buffer)
{
    GoatPenetrationConditionBuffer *buffer = (GoatPenetrationConditionBuffer *)skill_buffer;
    buffer->stacks = 1;
    buffer->rounds_left = 2;
}

bool_t goat_penetration_condition_trigger(const SkillCommand *command)
{
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_END_OF_ROUND:
    case COMBAT_EVENT_CONDITION_REAPPLY:
        return TRUE;
    case COMBAT_EVENT_ENGINE_ATTACK_DECLARATION:
    {
        const DmgCalcInstance *dmg_inst = combat_damage_peek_queue();
        return dmg_inst->defender.unit_id == command->caster.unit_id;
    }
    default:
        break;
    }

    return FALSE;
}

void goat_penetration_condition_execute(SkillCommand *command)
{
    CombatUnit *afflicted = combat_identifier_get_combat_unit(&(command->caster));
    GoatPenetrationConditionBuffer *buffer = (GoatPenetrationConditionBuffer *)(command->skill->skill_buffer);

    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_END_OF_ROUND:
        if (buffer->rounds_left != 0)
        {
            buffer->rounds_left -= 1;
        }
        else
        {
            buffer->stacks -= 1;
            if (buffer->stacks == 0)
            {
                combat_unit_remove_condition(
                    afflicted, command->skill->metadata, &(command->target));
            }
        }
        break;
    case COMBAT_EVENT_ENGINE_ATTACK_DECLARATION:
    {
        DmgCalcInstance *dmg_inst = combat_damage_peek_queue();
        dmg_inst->damage += buffer->stacks;

        tb_printf(&(print_window.buffer), 0x008888FF,
                  L"%ls's armor is shredded by %u!\n",
                  afflicted->unit->name, buffer->stacks);
        break;
    }
    case COMBAT_EVENT_CONDITION_REAPPLY:
        buffer->stacks += 1;
        break;
    default:
        break;
    }
}

SkillMetadata goat_penetration_condition_meta = {
    .id = SKILL_ID_GOAT_PENETRATION_CONDITION_ID,
    .type = SKILL_TYPE_SPECIAL_CONDITION,
    .cost = 0,
    .name = L"Penetration (C)",
    .description = L"This unit has its defense reduced",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {
        .skill_buffer_size = sizeof(GoatPenetrationConditionBuffer),
        .initialize_cb = goat_penetration_condition_initialize},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_penetration_condition_trigger,
    .execute_cb = goat_penetration_condition_execute};

bool_t goat_penetration_trigger(const SkillCommand *command)
{
    const DmgCalcInstance *dmg_inst = combat_damage_peek_queue();
    return command->event == COMBAT_EVENT_ENGINE_ATTACK_COMPLETION &&
           dmg_inst->attacker.unit_id == command->caster.unit_id;
}

void goat_penetration_execute(SkillCommand *command)
{
    DmgCalcInstance *dmg_inst = combat_damage_peek_queue();
    CombatUnit *target = combat_identifier_get_combat_unit(&(dmg_inst->defender));

    combat_unit_apply_condition(
        target, &goat_penetration_condition_meta, &(command->caster));
}

SkillMetadata goat_penetration_meta = {
    .id = SKILL_ID_GOAT_PENETRATION_ID,
    .type = SKILL_TYPE_PASSIVE,
    .cost = 0,
    .name = L"Penetration (P)",
    .description = L"On hit, decreases target defense",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {.skill_buffer_size = 0, .initialize_cb = NULL},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_penetration_trigger,
    .execute_cb = goat_penetration_execute};