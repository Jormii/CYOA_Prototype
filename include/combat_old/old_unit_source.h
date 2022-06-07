#ifndef UNIT_SOURCE_H
#define UNIT_SOURCE_H

#include "team.h"

typedef struct UnitSource_st
{
    size_t unit_id;
    Team *team;
    slot_t slot;
} UnitSource;

Unit *source_get_unit(UnitSource *source);
bool_t source_same_unit(const UnitSource *one, const UnitSource *other);

#endif