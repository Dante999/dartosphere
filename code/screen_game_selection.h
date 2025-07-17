#ifndef SCREEN_GAME_SELECTION_H
#define SCREEN_GAME_SELECTION_H

#include "screen.h"

void screen_game_selection_refresh(Screen *screen, Match *match);
void screen_game_selection_on_enter(Screen *screen, Match *match);
void screen_game_selection_on_exit(Screen *screen, Match *match);

#endif // SCREEN_GAME_SELECTION_H
