#ifndef COMBAT_ENGINE_H
#define COMBAT_ENGINE_H

#include "combat_team.h"
#include "dynamic_list.h"

typedef void (*OnEvent_fp)(const SkillCommand *event_command);

struct
{
    bool_t in_combat;
    CombatTeam players_team;
    CombatTeam enemy_team;
    DynamicList skills_queue;

    OnEvent_fp on_event_cb;
} combat_engine;

void combat_engine_initialize();

void combat_engine_broadcast_engine_event(CombatEvent event);
void combat_engine_broadcast_event(SkillCommand *event_command);

void combat_engine_add_command_to_queue(const SkillCommand *command);
void combat_engine_remove_queue_tail();
void combat_engine_execute_queue();

void combat_engine_format_active_command(
    Skill *skill, const CombatIdentifier *caster, const CombatIdentifier *target,
    SkillCommand *out_command);
void combat_engine_format_passive_command(
    Skill *skill, const CombatIdentifier *caster, const CombatIdentifier *target,
    CombatEvent event, const SkillCommand *cause, SkillCommand *out_command);
void combat_engine_format_condition_command(
    Skill *skill, const CombatIdentifier *afflicted, const CombatIdentifier *caused_by,
    CombatEvent condition_event, SkillCommand *out_command);

void combat_engine_combat_identifier_from_combat_unit(const CombatUnit *combat_unit, CombatIdentifier *out_identifier);

#endif