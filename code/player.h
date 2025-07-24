#ifndef PLAYER_H
#define PLAYER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PLAYER_NAME_LEN 10
#define MAX_PLAYER_COUNT     5



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
	char name[MAX_PLAYER_NAME_LEN];
	struct Turn turn;
	uint16_t score;
	uint16_t legs_won;
};

struct Player_List {
	struct Player items[MAX_PLAYER_COUNT];
	size_t count;
	int    index_active_player;
};


void player_list_init(struct Player_List *players);
void player_list_select_next(struct Player_List *players);
void player_list_select_previous(struct Player_List *players);
void player_list_add_player(struct Player_List *players);
void player_list_remove_player(struct Player_List *players);

bool player_next_dart_throw(struct Player *player);
bool player_previous_dart_throw(struct Player *player);
void player_clear_dart_throws(struct Player *player);

#endif // PLAYER_H
