#include <stdlib.h>

#include "utils.h"
#include "fixed_list.h"

byte_t *ptr_to_element(const FixedList *list, size_t index);

void fixed_list_init(FixedList *list, size_t length, size_t element_size)
{
    list->length = 0;
    list->max_length = length;

    list->buffer = malloc(length * element_size);
    list->element_size = element_size;
}

void fixed_list_deinit(FixedList *list)
{
    free(list->buffer);
}

byte_t *fixed_list_get(FixedList *list, size_t index)
{
    if (index >= list->length)
    {
        return NULL;
    }

    return ptr_to_element(list, index);
}

bool_t fixed_list_append(FixedList *list, const byte_t *data)
{
    if (list->length == list->max_length)
    {
        return FALSE;
    }

    byte_t *dst = ptr_to_element(list, list->length);
    copy_buffer(data, dst, list->element_size);
    list->length += 1;

    return TRUE;
}

bool_t fixed_list_remove(FixedList *list, size_t index)
{
    if (index >= list->length)
    {
        return FALSE;
    }

    byte_t *dst = ptr_to_element(list, index);
    for (size_t i = index + 1; i < list->length; ++i)
    {
        byte_t *src = ptr_to_element(list, i);
        copy_buffer(src, dst, list->element_size);

        dst = src;
    }

    list->length -= 1;

    return TRUE;
}

void fixed_list_clear(FixedList *list)
{
    list->length = 0;
}

void fixed_list_bubble_sort(FixedList *list, FixedListCompare_fp compare_cb)
{
    size_t n = list->length;
    if (n <= 1)
    {
        return;
    }

    for (size_t i = 0; i < (n - 1); ++i)
    {
        bool_t swapped = FALSE;

        for (size_t j = 0; j < (n - 1 - i); ++j)
        {
            byte_t *first = fixed_list_get(list, j);
            byte_t *second = fixed_list_get(list, j + 1);
            if (!compare_cb(first, second))
            {
                swap_buffer(first, second, list->element_size);
                swapped = TRUE;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}

byte_t *ptr_to_element(const FixedList *list, size_t index)
{
    return list->buffer + index * list->element_size;
}