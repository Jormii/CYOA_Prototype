#ifndef BEHAVIOURAL_H
#define BEHAVIOURAL_H

#include "types.h"

typedef void (*BehaviouralInitialize_fp)(void *buffer);
typedef void (*BehaviouralUpdate_fp)(void *buffer);

typedef struct Behavioural_st
{
    void *buffer;
    size_t buffer_size;
    BehaviouralInitialize_fp init_cb;
    BehaviouralUpdate_fp update_cb;
} Behavioural;

void behavioural_initialize(Behavioural *behavioural);
void behavioural_update(Behavioural *behavioural);

#endif