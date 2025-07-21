#ifndef SCREEN_GAME_X01_H
#define SCREEN_GAME_X01_H

// forwared declarations
struct Screen;
struct Match;

void screen_game_x01_refresh(struct Screen *screen, struct Match *match);
void screen_game_x01_on_enter(struct Screen *screen, struct Match *match);
void screen_game_x01_on_exit(struct Screen *screen, struct Match *match);

#endif // SCREEN_GAME_X01_H
