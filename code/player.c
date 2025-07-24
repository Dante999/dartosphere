#include "player.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "libcutils/util_makros.h"

static int get_field_type_multiplier(enum Field_Type type)
{
	switch(type) {
		case DARTS_SINGLE: return 1;
		case DARTS_DOUBLE: return 2;
		case DARTS_TRIPPLE: return 3;
		default: assert(false && "unhandled darts FIELD_TYPE");
	}
}

void player_list_init(struct Player_List *players)
{
	for (size_t i=0; i < MAX_PLAYER_COUNT; ++i) {
		snprintf(players->items[i].name,
			MAX_PLAYER_NAME_LEN,
			"Player %zu",
			i+1);
	}

	players->count = 1;
}



void player_list_add_player(struct Player_List *players)
{
	if (players->count < MAX_PLAYER_COUNT) {
		players->count++;
	}
}

void player_list_remove_player(struct Player_List *players)
{
	if (players->count > 1) {
		players->count -= 1;
	}
}



void player_list_select_next(struct Player_List *players)
{
	players->index_active_player++;
	players->index_active_player %= players->count;
}

void player_list_select_previous(struct Player_List *players)
{
	if (players->index_active_player > 0) {
		players->index_active_player--;
	}
	else {
		players->index_active_player = players->count-1;
	}

}

bool player_next_dart_throw(struct Player *player)
{
	if (player->turn.index_active_dart < (int)ARRAY_SIZE(player->turn.dart)-1) {
		player->turn.index_active_dart++;
		return true;
	}

	return false;
}

bool player_previous_dart_throw(struct Player *player)
{
	if (player->turn.index_active_dart > 0) {
		player->turn.index_active_dart--;
		return true;
	}

	return false;
}

void player_clear_dart_throws(struct Player *player)
{
	player->turn.index_active_dart = 0;

	for (size_t i=0; i < ARRAY_SIZE(player->turn.dart); ++i) {
		struct Dart_Hit *hit = &player->turn.dart[i];
		hit->field_type  = DARTS_SINGLE;
		hit->field_value = DART_FIELD_VALUE_UNSET;
	}
}


struct Player *player_list_get_active_player(struct Player_List *players)
{
	return &players->items[players->index_active_player];
}


int player_get_score_from_turn(struct Turn *turn)
{
	int score = 0;

	for (size_t i=0; i < ARRAY_SIZE(turn->dart); ++i) {
		struct Dart_Hit *hit = &turn->dart[i];

		if (hit->field_value == DART_FIELD_VALUE_UNSET) continue;

		score += (hit->field_value * get_field_type_multiplier(hit->field_type));
	}

	return score;
}
