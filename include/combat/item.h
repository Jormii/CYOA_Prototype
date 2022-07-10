#ifndef ITEM_H
#define ITEM_H

#include "types.h"
#include "skill.h"

typedef struct Item_st
{
    uint8_t amount;
    uint8_t limit;
    Skill skill;
} Item;

typedef struct Inventory_st
{
    size_t n_items;
    Item **items;
} Inventory;

#endif