#include "utils.h"
#include "dynamic_list.h"

void dynamic_list_init(DynamicList *list, size_t initial_length,
                       size_t length_increment, size_t element_size)
{
    list->length_increment = length_increment;
    fixed_list_init(&(list->fixed_list), initial_length, element_size);
}

void dynamic_list_deinit(DynamicList *list)
{
    fixed_list_deinit(&(list->fixed_list));
}

void dynamic_list_append(DynamicList *list, const byte_t *data)
{
    bool_t appended = fixed_list_append(&(list->fixed_list), data);
    if (!appended)
    {
        size_t new_length = list->fixed_list.length + list->length_increment;
        if (list->length_increment == DYNAMIC_LIST_INCREMENT_DOUBLE)
        {
            new_length *= 2;
        }

        // Allocate memory and copy the content in buffer
        byte_t *new_buffer = malloc(new_length * list->fixed_list.element_size);

        byte_t *src = list->fixed_list.buffer;
        size_t old_buffer_size = list->fixed_list.max_length * list->fixed_list.element_size;
        copy_buffer(src, new_buffer, old_buffer_size);

        // Free previous buffer
        fixed_list_deinit(&(list->fixed_list));

        // Update variables
        list->fixed_list.max_length = new_length;
        list->fixed_list.buffer = new_buffer;

        // Append data
        fixed_list_append(&(list->fixed_list), data);
    }
}