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
		match->legs_for_win = 3;
		match->round        = 0;
		break;

	case GAME_STATE_SELECT_PLAYERS:
		match_set_status(match, "Add players for your match");
		break;

	case GAME_STATE_GAME_ON:
		match_set_status(match, "Game on!");
	}

	match->state = state;
}
