#ifndef COMBAT_IDENTIFIER_H
#define COMBAT_IDENTIFIER_H

#include "types.h"

struct CombatUnit_st;
struct CombatTeam_st;

typedef struct CombatIdentifier_st
{
    size_t unit_id;                    // Unit identifier
    combat_slot_t unit_slot;           // What slot the unit occupied
    struct CombatTeam_st *combat_team; // What team the unit belongs to
} CombatIdentifier;

bool_t combat_identifier_still_deployed(const CombatIdentifier *identifier);
struct CombatUnit_st *combat_identifier_get_combat_unit(const CombatIdentifier *identifier);

#endif