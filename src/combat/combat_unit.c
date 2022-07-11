#include "combat_unit.h"
#include "combat_engine.h"

void combat_unit_initialize(CombatUnit *combat_unit)
{
    combat_unit->unit = NULL;
    combat_unit->died_this_round = FALSE;
    dynamic_list_init(&(combat_unit->special_conditions), 2, 2, sizeof(SpecialCondition));
}

bool_t combat_unit_tag_as_dead(CombatUnit *combat_unit)
{
    // Returns if the unit was alive
    bool_t died_this_round = combat_unit->died_this_round;
    combat_unit->died_this_round = TRUE;

    return !died_this_round;
}

void combat_unit_apply_condition(CombatUnit *combat_unit, const SkillMetadata *metadata, const CombatIdentifier *caused_by)
{
    SpecialCondition *existing_condition = combat_unit_afflicted_by_condition(combat_unit, metadata, caused_by);
    if (existing_condition != NULL)
    {
        CombatIdentifier afflicted;
        combat_engine_combat_identifier_from_combat_unit(combat_unit, &afflicted);
        special_condition_reapply(existing_condition, &afflicted);
    }
    else
    {
        SpecialCondition condition;
        skill_initialize(&(condition.skill), metadata);
        combat_identifier_copy(caused_by, &(condition.caused_by));

        dynamic_list_append(&(combat_unit->special_conditions), (byte_t *)&condition);
    }
}

void combat_unit_remove_condition(CombatUnit *combat_unit, const SkillMetadata *metadata, const CombatIdentifier *caused_by)
{
    // TODO: Duplicated code :: combat_unit_afflicted_by_condition
    FixedList *conditions = &(combat_unit->special_conditions.fixed_list);
    for (size_t i = 0; i < conditions->length; ++i)
    {
        SpecialCondition *condition = (SpecialCondition *)fixed_list_get(conditions, i);
        if (special_condition_are_equivalent(condition, metadata, caused_by))
        {
            CombatIdentifier afflicted;
            combat_engine_combat_identifier_from_combat_unit(combat_unit, &afflicted);

            SkillCommand condition_command;
            combat_engine_format_condition_command(
                &(condition->skill), &afflicted, caused_by, COMBAT_EVENT_CONDITION_REMOVAL,
                &condition_command);

            // Comunicate it's being removed
            if (metadata->trigger_cb(&condition_command))
            {
                metadata->execute_cb(&condition_command);
            }

            fixed_list_remove(conditions, i);        // Remove from unit's conditions
            skill_deinitialize(&(condition->skill)); // Free resources
            break;
        }
    }
}

stat_t combat_unit_calculate_stat(const CombatUnit *combat_unit, Stat stat)
{
    // TODO: Take into account special conditions
    stat_t base = unit_calculate_stat(combat_unit->unit, stat);
    return base;
}

bool_t combat_unit_is_valid(const CombatUnit *combat_unit)
{
    return combat_unit->unit != NULL;
}

SpecialCondition *combat_unit_afflicted_by_condition(CombatUnit *combat_unit, const SkillMetadata *metadata, const CombatIdentifier *caused_by)
{
    FixedList *conditions = &(combat_unit->special_conditions.fixed_list);
    for (size_t i = 0; i < conditions->length; ++i)
    {
        SpecialCondition *condition = (SpecialCondition *)fixed_list_get(conditions, i);
        if (special_condition_are_equivalent(condition, metadata, caused_by))
        {
            return condition;
        }
    }

    return NULL;
}