#ifndef BINARY_IMAGE_H
#define BINARY_IMAGE_H

#include "types.h"
#include "screen_buffer.h"

typedef struct BinaryImage_st
{
    // TODO: Can save space using bit masks
    screen_t width;
    screen_t height;
    u8_t mask[];
} BinaryImage;

void binary_image_display(const BinaryImage *image, screen_t x, screen_t y);

#endif