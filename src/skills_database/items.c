#include "all_species.h"

extern Item item_heal;

Item *items_all[] = {
    &item_heal};

Inventory inventory = {
    .n_items = sizeof(items_all) / sizeof(Item *),
    .items = items_all};