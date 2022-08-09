#ifndef STORY_SCREEN_H
#define STORY_SCREEN_H

#include "window.h"

extern Window story_messages_window;
extern Window story_choices_window;

void story_screen_initialize();
void story_screen_update();

#endif