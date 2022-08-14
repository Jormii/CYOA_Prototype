#include "story_screen.h"
#include "base_set_font.h"
#include "screen_buffer.h"
#include "screen_common.h"
#include "window_display.h"
#include "creature_summary.h"

#define STORY_MESSAGES_BUFFER_LENGTH 1024
#define STORY_CHOICES_BUFFER_LENGTH 128

Window story_messages_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_MARGIN,
        .bottom = SCREEN_COMMON_HOR_DIV_Y - SCREEN_EXTRA_MARGIN},
    .font = &base_set_font};

Window story_choices_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_HOR_DIV_Y + SCREEN_EXTRA_MARGIN,
        .bottom = SCREEN_HEIGHT - SCREEN_COMMON_MARGIN},
    .font = &base_set_font};

void story_screen_initialize()
{
    create_text_buffer(STORY_MESSAGES_BUFFER_LENGTH, &(story_messages_window.buffer));
    create_text_buffer(STORY_CHOICES_BUFFER_LENGTH, &(story_choices_window.buffer));
}

void story_screen_update()
{
    display_window(&story_messages_window);
    display_window(&story_choices_window);
    creature_summary_update();
    screen_common_update();
}