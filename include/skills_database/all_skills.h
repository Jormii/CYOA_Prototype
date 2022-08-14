#ifndef ALL_SKILLS_H
#define ALL_SKILLS_H

#include "item.h"
#include "skill.h"
#include "text_buffer.h"
#include "combat_engine.h"

extern TextBuffer *skills_buffer;

typedef enum SkillIdentifier_en
{
    // Items
    SKILL_ID_ITEM_HEAL_ID,

    // Goat
    SKILL_ID_GOAT_KICK_ID,
    SKILL_ID_GOAT_RAM_ID,
    SKILL_ID_GOAT_UNSTOPPABLE_ID,
    SKILL_ID_GOAT_FOCUS_ID,
    SKILL_ID_GOAT_FOCUS_CONDITION_ID,
    SKILL_ID_GOAT_CHARGE_ID,
    SKILL_ID_GOAT_DESPERATION_ID,
    SKILL_ID_GOAT_PENETRATION_ID,
    SKILL_ID_GOAT_PENETRATION_CONDITION_ID,
    SKILL_ID_GOAT_RETALIATION_ID,
    SKILL_ID_GOAT_RETALIATION_CONDITION_ID,
    SKILL_ID_GOAT_ANTICIPATION_ID
} SkillIdentifier;

#endif