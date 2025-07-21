#ifndef SCREEN_PLAYER_SELECTION_H
#define SCREEN_PLAYER_SELECTION_H

// forwared declarations
struct Screen;
struct Match;

void screen_player_selection_on_enter(struct Screen *screen, struct Match *match);
void screen_player_selection_refresh(struct Screen *screen, struct Match *match);
void screen_player_selection_on_exit(struct Screen *screen, struct Match *match);

#endif // SCREEN_PLAYER_SELECTION_H
