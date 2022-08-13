#include "binary_image.h"

void binary_image_display(const BinaryImage *image, screen_t x, screen_t y)
{
    size_t image_index = 0;
    size_t buffer_index = TEXT_BUFFER_INDEX(x, y);
    size_t next_row_buffer_index = buffer_index + BUFFER_WIDTH;

    for (screen_t column = 0; column < image->height; ++column)
    {
        for (screen_t row = 0; row < image->width; ++row)
        {
            draw_buffer[buffer_index] = image->mask[image_index] * 0x00FFFFFF;

            image_index += 1;
            buffer_index += 1;
        }

        buffer_index = next_row_buffer_index;
        next_row_buffer_index += BUFFER_WIDTH;
    }
}