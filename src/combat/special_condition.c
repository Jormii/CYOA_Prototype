#include "combat_engine.h"
#include "special_condition.h"

void special_condition_reapply(SpecialCondition *condition, const CombatIdentifier *afflicted)
{
    SkillCommand condition_command;
    combat_engine_format_condition_command(
        &(condition->skill), afflicted, &(condition->caused_by),
        COMBAT_EVENT_CONDITION_REAPPLY, &condition_command);

    // TODO: Should be broadcasted?
    const SkillMetadata *metadata = condition->skill.metadata;
    if (metadata->trigger_cb(&condition_command))
    {
        metadata->execute_cb(&condition_command);
    }
}

bool_t special_condition_are_equivalent(const SpecialCondition *condition, const SkillMetadata *metadata, const CombatIdentifier *caused_by)
{
    return condition->skill.metadata->id == metadata->id &&
           condition->caused_by.unit_id == caused_by->unit_id;
}