#include "darts.h"

#include "string.h"

char field_type_as_char(Field_Type type)
{
	switch(type) {
		case DARTS_SINGLE : return 'S';
		case DARTS_DOUBLE : return 'D';
		case DARTS_TRIPPLE: return 'T';
	}

	return '?';
}



void match_init(Match *match)
{
	match->player_list.count = 0;
	match_add_player(match, "Player 1");

	match->legs_for_win = DEFAULT_LEGS_FOR_WIN;
	match->round        = 0;
}



void match_add_player(Match *match, const char *player_name)
{
	Player_List *player_list = &match->player_list;

	if (player_list->count < MAX_PLAYER_COUNT) {
		strncpy(player_list->items[player_list->count].name, player_name, MAX_PLAYER_NAME_LEN);
		player_list->count++;
	}
}

void match_remove_player(Match *match)
{
	if (match->player_list.count > 1) {
		match->player_list.count -= 1;
	}
}


void match_set_game_mode_list(Match *match, Game_Mode *modes, size_t modes_count)
{
	match->game_mode_list.items = modes;
	match->game_mode_list.count = modes_count;
}

