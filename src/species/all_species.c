#include "types.h"
#include "all_species.h"

ACTIVE_INIT(active_skill_init_do_nothing)
{
}

ACTIVE_DEINIT(active_skill_deinit_do_nothing)
{
}

ACTIVE_CAN_BE_USED(active_skill_can_be_used_always)
{
    return TRUE;
}

PASSIVE_INIT(passive_skill_init_do_nothing)
{
}

PASSIVE_DEINIT(passive_skill_deinit_do_nothing)
{
}

PASSIVE_CAN_BE_USED(passive_skill_can_be_used_always)
{
    return TRUE;
}