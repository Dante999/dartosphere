#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <stddef.h>

// forward declarations
struct Screen;
struct Match;



typedef enum {
	GAME_SCREEN_WELCOME,
	GAME_SCREEN_SELECT_PLAYERS,
	GAME_SCREEN_SELECT_GAME,
	GAME_SCREEN_GAME_ON
} Game_Screen_Id;

typedef struct {
	Game_Screen_Id id;
	void (*on_enter)(struct Screen *screen , struct Match *match);
	void (*refresh)(struct Screen *screen  , struct Match *match);
	void (*on_exit)(struct Screen *screen  , struct Match *match);
	Game_Screen_Id next_screen;
	Game_Screen_Id previous_screen;
} Game_Screen;

typedef struct {
	Game_Screen *items;
	size_t       count;
	int          index_active_screen;
} Game_Screen_List;


void game_screen_init(struct Screen *screen);
void game_screen_refresh(struct Screen *screen, struct Match *match);
void game_screen_previous(struct Screen *screen, struct Match *match);
void game_screen_next(struct Screen *screen, struct Match *match);
void game_screen_set_header(struct Screen *screen, const char *first_line, const char *second_line, const char *third_line);
void game_screen_set_status(struct Screen *screen, const char *status);

Game_Screen *game_screen_get_current(struct Screen *screen);

#endif
