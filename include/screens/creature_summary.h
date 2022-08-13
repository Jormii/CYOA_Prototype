#ifndef CREATURE_SUMMARY_H
#define CREATURE_SUMMARY_H

#include "window.h"
#include "text_buffer.h"

struct
{
    size_t creature_index;
    Window display_window;
    size_t attr_points_available;
} creature_summary;

void creature_summary_initialize();
void creature_summary_update();

#endif