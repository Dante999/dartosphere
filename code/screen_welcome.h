#ifndef SCREEN_WELCOME_H
#define SCREEN_WELCOME_H

#include "screen.h"

void screen_welcome_on_enter(struct Screen *screen, struct Match *match);
void screen_welcome_on_exit(struct Screen *screen, struct Match *match);
void screen_welcome_refresh(struct Screen *screen, struct Match *match);

#endif // SCREEN_WELCOME_H
