#include "darts.h"

#include "string.h"

char field_type_as_char(enum Field_Type type)
{
	switch(type) {
		case DARTS_SINGLE : return 'S';
		case DARTS_DOUBLE : return 'D';
		case DARTS_TRIPPLE: return 'T';
	}

	return '?';
}



void match_init(struct Match *match)
{
	player_list_init(&match->player_list);

	match->legs_for_win = DEFAULT_LEGS_FOR_WIN;
	match->round        = 0;
}

void match_set_game_mode_list(struct Match *match, struct Game_Mode *modes, size_t modes_count)
{
	match->game_mode_list.items = modes;
	match->game_mode_list.count = modes_count;
}

