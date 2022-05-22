#ifndef SPECIAL_CONDITION_H
#define SPECIAL_CONDITION_H

#include "types.h"
#include "combat_event.h"

struct SpecialCondition_st;
struct SpecialConditionInfo_st;
struct SpecialConditionCommand_st;

typedef void (*SpecialConditionInit_fp)(struct SpecialCondition_st *condition,
                                        const struct SpecialConditionInfo_st *info);
typedef void (*SpecialConditionDeinit_fp)(struct SpecialCondition_st *condition);
typedef void (*SpecialConditionExecute_fp)(struct SpecialConditionCommand_st *command);
typedef void (*SpecialConditionReapply_fp)(struct SpecialCondition_st *existing_contion,
                                           struct SpecialConditionCommand_st *command);
typedef bool_t (*SpecialConditionWoreOff_fp)(const struct SpecialCondition_st *condition);

typedef struct SpecialConditionInfo_st
{
    size_t id;
    event_flags_t triggers;
    SpecialConditionInit_fp init_cb;
    SpecialConditionDeinit_fp deinit_cb;
    SpecialConditionExecute_fp execute_cb;
    SpecialConditionReapply_fp reapply_cb;
    SpecialConditionWoreOff_fp wore_off_cb;
} SpecialConditionInfo;

typedef struct SpecialCondition_st
{
    UnitSource caused_by;
    const SpecialConditionInfo *condition_info;
    void *condition_buffer;
} SpecialCondition;

typedef struct SpecialConditionCommand_st
{
    UnitSource affected;
    CombatEventSource *source;
    SpecialCondition *special_condition;
} SpecialConditionCommand;

#endif