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

void match_set_status(Match *match, const char *status)
{
	strncpy(match->status_text, status, sizeof(match->status_text));
}


void match_init(Match *match)
{
	match->player_count = 0;
	match_add_player(match, "Player 1");

	match->legs_for_win = 3;
	match->round        = 0;
}


//void match_previous_state(Match *match) {
//	switch(match->state) {
//	case GAME_STATE_WELCOME:
//		// there is no previous state
//		break;
//
//	case GAME_STATE_SELECT_PLAYERS:
//		match->state = GAME_STATE_WELCOME;
//		break;
//
//	case GAME_STATE_SELECT_GAME:
//		match->state = GAME_STATE_SELECT_PLAYERS;
//		break;
//
//	case GAME_STATE_GAME_ON:
//		match->state = GAME_STATE_SELECT_GAME;
//		break;
//
//	default:
//		match->state = GAME_STATE_SELECT_PLAYERS;
//		break;
//
//	}
//}


//void match_next_state(Match *match)
//{
//	switch(match->state) {
//	case GAME_STATE_WELCOME:
//		match->state = GAME_STATE_SELECT_PLAYERS;
//		break;
//
//	case GAME_STATE_SELECT_PLAYERS:
//		match->state = GAME_STATE_SELECT_GAME;
//		break;
//
//	case GAME_STATE_SELECT_GAME:
//		match->state = GAME_STATE_GAME_ON;
//		break;
//
//	case GAME_STATE_GAME_ON:
//		match->state = GAME_STATE_SELECT_PLAYERS;
//		break;
//
//	default:
//		match->state = GAME_STATE_GAME_ON;
//		break;
//
//	}
//}

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


void match_set_available_game_modes(Match *match, Game_Mode *modes, size_t modes_count)
{
	match->available_game_modes.game_modes       = modes;
	match->available_game_modes.game_modes_count = modes_count;
}


void match_set_status_message(Match *match, const char *msg)
{
	strncpy(match->status_text, msg, sizeof(match->status_text));
}
