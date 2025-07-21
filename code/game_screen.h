#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <stddef.h>

struct screen;
struct match;



typedef enum {
	GAME_SCREEN_WELCOME,
	GAME_SCREEN_SELECT_PLAYERS,
	GAME_SCREEN_SELECT_GAME,
	GAME_SCREEN_GAME_ON
} Game_Screen_Id;

typedef struct {
	Game_Screen_Id id;
	void (*on_enter)(struct screen *screen, struct match *match);
	void (*refresh)(struct screen *screen, struct match *match);
	void (*on_exit)(struct screen *screen, struct match *match);
	Game_Screen_Id next_screen;
	Game_Screen_Id previous_screen;
} Game_Screen;

typedef struct {
	Game_Screen *items;
	size_t       count;
	int          index_active_screen;
} Game_Screen_List;


void game_screen_init(struct screen *screen);
void game_screen_refresh(struct screen *screen, struct match *match);
void game_screen_previous(struct screen *screen, struct match *match);
void game_screen_next(struct screen *screen, struct match *match);

Game_Screen *game_screen_get_current(struct screen *screen);

#endif
