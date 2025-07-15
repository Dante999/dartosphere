#include "darts.h"

#include "string.h"

char multiplicator_as_char(Multiplicator m)
{
	switch(m) {
		case DARTS_SINGLE : return 'S';
		case DARTS_DOUBLE : return 'D';
		case DARTS_TRIPPLE: return 'T';
	}

	return '?';
}

void match_set_status(Match *match, const char *status)
{
	strncpy(match->status_text, status, sizeof(match->status_text));
}


void match_set_state(Match *match, Game_State state)
{
	switch(state) {

	case GAME_STATE_WELCOME:
		match_set_status(match, "Press <ENTER> to continue");
		match->player_count = 0;
		match_add_player(match, "Player 1");

		match->legs_for_win = 3;
		match->round        = 0;
		break;

	case GAME_STATE_SELECT_PLAYERS:
		match_set_status(match, "");
		break;

	case GAME_STATE_SELECT_GAME:
	case GAME_STATE_GAME_ON:
		match_set_status(match, "Game on!");
	}

	match->state = state;
}

void match_add_player(Match *match, const char *player_name)
{
	if (match->player_count < MAX_PLAYER_COUNT) {
		strncpy(match->players[match->player_count].name, player_name, MAX_PLAYER_NAME_LEN);
		match->player_count++;
	}
}

void match_remove_player(Match *match)
{
	if (match->player_count > 1) {
		match->player_count -= 1;
	}
}
