#ifndef SCREEN_GAME_SELECTION_H
#define SCREEN_GAME_SELECTION_H

#include "screen.h"

void screen_game_selection_refresh(struct Screen *screen, struct Match *match);
void screen_game_selection_on_enter(struct Screen *screen, struct Match *match);
void screen_game_selection_on_exit(struct Screen *screen, struct Match *match);

#endif // SCREEN_GAME_SELECTION_H
