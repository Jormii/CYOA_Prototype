#include "ui.h"
#include "all_skills.h"
#include "fixed_list.h"
#include "combat_damage.h"

typedef struct GoatAnticipationBuffer_st
{
    CombatIdentifier target;
    FixedList attacked_by_ids;
} GoatAnticipationBuffer;

void goat_anticipation_buffer_reset(GoatAnticipationBuffer *buffer);
void goat_anticipation_buffer_store_target(GoatAnticipationBuffer *buffer, const SkillCommand *command);
void goat_anticipation_buffer_store_attacker(GoatAnticipationBuffer *buffer, size_t unit_id);
void goat_anticipation_buffer_counter_attack(const GoatAnticipationBuffer *buffer, SkillCommand *command);
bool_t goat_anticipation_buffer_attacker_is_stored_target(const GoatAnticipationBuffer *buffer,
                                                          const SkillCommand *command, const DmgCalcInstance *dmg_inst);
bool_t goat_anticipation_buffer_attacked_by(GoatAnticipationBuffer *buffer, size_t unit_id);

void goat_anticipation_buffer_reset(GoatAnticipationBuffer *buffer)
{
    buffer->target.unit_id = UNDEFINED_UNIT_ID;
    fixed_list_clear(&(buffer->attacked_by_ids));
}

void goat_anticipation_buffer_store_target(GoatAnticipationBuffer *buffer, const SkillCommand *command)
{
    combat_identifier_copy(&(command->target), &(buffer->target));
}

void goat_anticipation_buffer_store_attacker(GoatAnticipationBuffer *buffer, size_t unit_id)
{
    fixed_list_append(&(buffer->attacked_by_ids), (byte_t *)&unit_id);
}

void goat_anticipation_buffer_counter_attack(const GoatAnticipationBuffer *buffer, SkillCommand *command)
{
    DmgCalcInstance *dmg_instance = combat_damage_declare_attack(
        &(command->caster), &(buffer->target), command);
    combat_damage_perform(dmg_instance);

    const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));
    const CombatUnit *target = combat_identifier_get_combat_unit(&(buffer->target));
    tb_printf(&(print_window.buffer), 0x00FFFFFF, L"%ls anticipates %ls's attack!\n",
              caster->unit->name, target->unit->name);
}

bool_t goat_anticipation_buffer_attacker_is_stored_target(const GoatAnticipationBuffer *buffer,
                                                          const SkillCommand *command, const DmgCalcInstance *dmg_inst)
{
#if 0
    LOG("--\n");
    LOG("ID: %u\n", command->caster.unit_id);
    LOG("Attacker: %u <-> Targeted: %u\n", dmg_inst->attacker.unit_id, buffer->target.unit_id);
    LOG("Defender: %u <-> Self: %u\n", dmg_inst->defender.unit_id, command->caster.unit_id);
    LOG("--\n");
#endif

    return dmg_inst->attacker.unit_id == buffer->target.unit_id &&
           dmg_inst->defender.unit_id == command->caster.unit_id;
}

bool_t goat_anticipation_buffer_attacked_by(GoatAnticipationBuffer *buffer, size_t unit_id)
{
    FixedList *list = &(buffer->attacked_by_ids);
    for (size_t i = 0; i < list->length; ++i)
    {
        size_t *attacker_id = (size_t *)fixed_list_get(list, i);
        if (*attacker_id == unit_id)
        {
            return TRUE;
        }
    }

    return FALSE;
}

void goat_anticipation_initialize(void *skill_buffer)
{
    GoatAnticipationBuffer *buffer = (GoatAnticipationBuffer *)skill_buffer;
    buffer->target.unit_id = UNDEFINED_UNIT_ID;
    fixed_list_init(&(buffer->attacked_by_ids), 2 * MAX_UNITS_IN_COMBAT, sizeof(size_t));
}

bool_t goat_anticipation_trigger(const SkillCommand *command)
{
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_START_OF_ROUND:
        return TRUE;
    case COMBAT_EVENT_SKILL_EXECUTION:
        return skill_command_caster_is_cause_of_event(command) &&
               skill_metadata_targets_single_unit(command->cause.skill->metadata);
    case COMBAT_EVENT_UNIT_ATTACK_DECLARATION:
    {
        const DmgCalcInstance *dmg_instance = combat_damage_get_instance(
            command->cause.dmg_instance_id);
        GoatAnticipationBuffer *buffer = (GoatAnticipationBuffer *)(command->skill->skill_buffer);
        return dmg_instance->defender.unit_id == command->caster.unit_id &&
               !goat_anticipation_buffer_attacked_by(buffer, dmg_instance->attacker.unit_id);
    }
    default:
        break;
    }

    return FALSE;
}

void goat_anticipation_execute(SkillCommand *command)
{
    GoatAnticipationBuffer *buffer = (GoatAnticipationBuffer *)(command->skill->skill_buffer);
    switch (command->event)
    {
    case COMBAT_EVENT_ENGINE_START_OF_ROUND:
        goat_anticipation_buffer_reset(buffer);
        break;
    case COMBAT_EVENT_SKILL_EXECUTION:
        goat_anticipation_buffer_store_target(buffer, command);
        if (goat_anticipation_buffer_attacked_by(buffer, command->target.unit_id))
        {
            const CombatUnit *caster = combat_identifier_get_combat_unit(&(command->caster));

            tb_printf(&(print_window.buffer), 0x008888FF,
                      L"%ls isnt't as fast as its target and can't counter attack!\n",
                      caster->unit->name);
        }
        break;
    case COMBAT_EVENT_UNIT_ATTACK_DECLARATION:
    {
        const DmgCalcInstance *dmg_instance = combat_damage_get_instance(
            command->cause.dmg_instance_id);
        goat_anticipation_buffer_store_attacker(buffer, dmg_instance->attacker.unit_id);
        if (goat_anticipation_buffer_attacker_is_stored_target(buffer, command, dmg_instance))
        {
            goat_anticipation_buffer_counter_attack(buffer, command);
        }
        break;
    }
    default:
        break;
    }
}

SkillMetadata goat_anticipation_meta = {
    .id = SKILL_ID_GOAT_ANTICIPATION_ID,
    .type = SKILL_TYPE_PASSIVE,
    .cost = 0,
    .name = L"Anticipation (P)",
    .description = L"If this unit is attacked, it strikes the attacker before it happens",
    .priority = SKILL_PRIORITY_HIGHEST,

    .initialization = {
        .skill_buffer_size = sizeof(GoatAnticipationBuffer),
        .initialize_cb = goat_anticipation_initialize},
    .deinitialize_cb = NULL,
    .trigger_cb = goat_anticipation_trigger,
    .execute_cb = goat_anticipation_execute};