#include "story_screen.h"
#include "base_set_font.h"
#include "screen_buffer.h"
#include "window_display.h"

#define STORY_MESSAGES_BUFFER_LENGTH 1024
#define STORY_CHOICES_BUFFER_LENGTH 128

Window story_messages_window = {
    .margin = {
        .left = 10,
        .right = SCREEN_WIDTH - 10,
        .top = 10,
        .bottom = (SCREEN_HEIGHT >> 1) - 10},
    .font = &base_set_font};

Window story_choices_window = {
    .margin = {
        .left = 10,
        .right = SCREEN_WIDTH - 10,
        .top = (SCREEN_HEIGHT >> 1) + 10,
        .bottom = SCREEN_HEIGHT},
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
}