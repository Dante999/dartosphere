#ifndef SCREEN_GAME_GOTCHA_H
#define SCREEN_GAME_GOTCHA_H

// forwared declarations
struct Screen;
struct Match;

void screen_configure_game_gotcha_refresh(struct Screen *screen, struct Match *match);
void screen_configure_game_gotcha_on_enter(struct Screen *screen, struct Match *match);
void screen_configure_game_gotcha_on_exit(struct Screen *screen, struct Match *match);

void screen_play_game_gotcha_refresh(struct Screen *screen, struct Match *match);
void screen_play_game_gotcha_on_enter(struct Screen *screen, struct Match *match);
void screen_play_game_gotcha_on_exit(struct Screen *screen, struct Match *match);
#endif // SCREEN_GAME_GOTCHA_H
