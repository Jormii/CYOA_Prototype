#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "window.h"

extern Window title_name; // TODO: Sprite
extern Window title_new;
extern Window title_continue;

void title_screen_initialize();
void title_screen_update();

#endif