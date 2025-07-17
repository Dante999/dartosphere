#ifndef SCREEN_WELCOME_H
#define SCREEN_WELCOME_H

#include "screen.h"

void screen_welcome_on_enter(Screen *screen, Match *match);
void screen_welcome_on_exit(Screen *screen, Match *match);
void screen_welcome_refresh(Screen *screen, Match *match);

#endif // SCREEN_WELCOME_H
