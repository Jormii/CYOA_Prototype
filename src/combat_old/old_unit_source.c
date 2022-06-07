#include "unit_source.h"

Unit *source_get_unit(UnitSource *source)
{
    return team_get_unit(source->team, source->slot);
}

bool_t source_same_unit(const UnitSource *one, const UnitSource *other)
{
    if (one == NULL || other == NULL)
    {
        return FALSE;
    }

    return one->unit_id == other->unit_id;
}