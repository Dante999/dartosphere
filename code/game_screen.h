#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <stddef.h>

// forward declarations
struct Screen;
struct Match;



enum Game_Screen_Id {
	GAME_SCREEN_UNDEFINED,
	GAME_SCREEN_WELCOME,
	GAME_SCREEN_SELECT_PLAYERS,
	GAME_SCREEN_SELECT_GAME,
	GAME_SCREEN_GAME_X01
};

struct Game_Screen {
	enum Game_Screen_Id id;
	void (*on_enter)(struct Screen *screen , struct Match *match);
	void (*refresh)(struct Screen *screen  , struct Match *match);
	void (*on_exit)(struct Screen *screen  , struct Match *match);
	enum Game_Screen_Id next_screen;
	enum Game_Screen_Id previous_screen;
};

struct Game_Screen_List {
	struct Game_Screen *items;
	size_t count;
	int index_active_screen;
};

struct Game_Mode {
	char name[255];
	char description[255];
	enum Game_Screen_Id screen_id;
};

struct Game_Mode_List {
	struct Game_Mode *items;
	size_t count;
	int    index_active_mode;
};

void game_screen_init(struct Screen *screen, struct Match *match);
void game_screen_refresh(struct Screen *screen, struct Match *match);
void game_screen_previous(struct Screen *screen, struct Match *match);
void game_screen_next(struct Screen *screen, struct Match *match);
void game_screen_set_header(struct Screen *screen, const char *first_line, const char *second_line, const char *third_line);
void game_screen_set_status(struct Screen *screen, const char *status);

struct Game_Screen *game_screen_get_current(struct Screen *screen);

#endif
