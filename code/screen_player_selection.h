#ifndef SCREEN_PLAYER_SELECTION_H
#define SCREEN_PLAYER_SELECTION_H

#include "screen.h"

void screen_player_selection_on_enter(Screen *screen, Match *match);
void screen_player_selection_refresh(Screen *screen, Match *match);
void screen_player_selection_on_exit(Screen *screen, Match *match);

#endif // SCREEN_PLAYER_SELECTION_H
