#ifndef FIXED_LIST_H
#define FIXED_LIST_H

#include "types.h"

typedef struct FixedList_st
{
    size_t length;
    size_t max_length;

    byte_t *buffer;
    size_t element_size;
} FixedList;

void fixed_list_init(FixedList *list, size_t length, size_t element_size);
void fixed_list_deinit(FixedList *list);

byte_t *fixed_list_get(FixedList *list, size_t index);
bool_t fixed_list_append(FixedList *list, const byte_t *data);
bool_t fixed_list_remove(FixedList *list, size_t index);

#endif