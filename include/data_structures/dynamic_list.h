#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include "fixed_list.h"

#define DYNAMIC_LIST_INCREMENT_DOUBLE 0

typedef struct DynamicList_st
{
    FixedList fixed_list;
    size_t length_increment;
} DynamicList;

void dynamic_list_init(DynamicList *list, size_t initial_length,
                       size_t length_increment, size_t element_size);
void dynamic_list_deinit(DynamicList *list);

void dynamic_list_append(DynamicList *list, const byte_t *data);

#endif