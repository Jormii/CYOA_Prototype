#ifndef COMBAT_DESCRIPTOR_H
#define COMBAT_DESCRIPTOR_H

#include "types.h"

struct CombatTeam_st;

typedef struct CombatDescriptor_st
{
    size_t unit_id;                    // Unit identifier
    combat_slot_t unit_slot;           // What slot the unit occupied
    struct CombatTeam_st *combat_team; // What team the unit belongs to
} CombatDescriptor;

#endif