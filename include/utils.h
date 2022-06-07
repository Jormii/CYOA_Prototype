#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void copy_buffer(const byte_t *src, byte_t *dst, size_t size);
void swap_buffer(byte_t *b1, byte_t *b2, size_t size);

#endif