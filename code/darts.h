#ifndef DARTS_H
#define DARTS_H


#define MAX_PLAYER_NAME_LEN 10
#define MAX_PLAYER_COUNT     5
#define DEFAULT_LEGS_FOR_WIN 3

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "game_screen.h"



enum Field_Type {
	DARTS_SINGLE,
	DARTS_DOUBLE,
	DARTS_TRIPPLE
};

struct Dart_Hit{
	enum Field_Type field_type;
	int16_t    field_value;
};

struct Turn{
	struct Dart_Hit dart[3];
	int index_active_dart;
};




struct Player{
	char     name[MAX_PLAYER_NAME_LEN];
	struct Turn     turn;
	uint16_t score;
	uint16_t legs_won;
};

struct Player_List {
	struct Player items[MAX_PLAYER_COUNT];
	size_t count;
	int    index_active_player;
};

struct Match {
	struct Player_List    player_list;
	struct Game_Mode_List game_mode_list;
	size_t legs_for_win;
	size_t round;
};

char field_type_as_char(enum Field_Type type);
void match_init(struct Match *match);
void match_add_player(struct Match *match, const char *name);
void match_remove_player(struct Match *match);
void match_set_game_mode_list(struct Match *match, struct Game_Mode *modes, size_t modes_count);


#endif // DARTS_H
