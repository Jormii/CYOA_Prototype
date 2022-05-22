#include "utils.h"

void copy_buffer(const byte_t *src, byte_t *dst, size_t size)
{
    for (size_t byte = 0; byte < size; ++byte)
    {
        dst[byte] = src[byte];
    }
}

void swap_buffer(byte_t *b1, byte_t *b2, size_t size)
{
    for (size_t byte = 0; byte < size; ++byte)
    {
        byte_t tmp = b1[byte];
        b1[byte] = b2[byte];
        b2[byte] = tmp;
    }
}