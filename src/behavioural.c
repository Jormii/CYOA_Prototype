#include "behavioural.h"

void behavioural_initialize(Behavioural *behavioural)
{
    behavioural->buffer = NULL;
    if (behavioural->init_cb != NULL)
    {
        behavioural->buffer = malloc(behavioural->buffer_size);
        behavioural->init_cb(behavioural->buffer);
    }
}

void behavioural_update(Behavioural *behavioural)
{
    behavioural->update_cb(behavioural->buffer);
}