#include "game_x01.h"

const char *game_x01_score_as_string(enum X01_Score score)
{
	switch(score) {
		case X01_SCORE_301: return "301";
		case X01_SCORE_501: return "501";
		case X01_SCORE_701: return "701";
		default: return "<?>";
	}
}

const char *game_x01_mode_as_string(enum X01_Mode mode)
{
	switch(mode) {
		case X01_MODE_STRAIGHT : return "Straight";
		case X01_MODE_DOUBLE   : return "Double";
		case X01_MODE_MASTER   : return "Master";
		default: return "<?>";
	}
}


int game_x01_get_start_score_as_int(struct Game_X01 *game)
{
	switch(game->start_score) {
		case X01_SCORE_301: return 301;
		case X01_SCORE_501: return 501;
		case X01_SCORE_701: return 701;
		default: return 0;
	}
}

static bool game_x01_is_mode_satisfied_by_hit(enum X01_Mode mode, struct Dart_Hit *hit)
{
	switch(mode) {
		case X01_MODE_STRAIGHT: return true;

		case X01_MODE_DOUBLE:
			return (hit->field_type == DARTS_DOUBLE) ? true : false;

		case X01_MODE_MASTER:
			return (hit->field_type == DARTS_DOUBLE || hit->field_type == DARTS_TRIPPLE)
				? true : false;
		default:
			return false;
	}
}

enum X01_Result game_x01_register_dart_throw(struct Game_X01 *game, struct Player *player)
{
	struct Dart_Hit *hit = player_get_current_dart_throw(player);

	const int score_turn = player_get_score_from_current_turn(player);
	const int score_delta = player->score - score_turn;

	if (score_delta < 0) {
		return X01_RESULT_PLAYER_OVERSHOOT;
	}
	if (score_delta == 0) {
		return game_x01_is_mode_satisfied_by_hit(game->check_out, hit) ? 
			X01_RESULT_PLAYER_WON : X01_RESULT_CHECKOUT_NOT_SATISFIED;
	}
	if (score_delta == 1) {
		return game->check_out == X01_MODE_STRAIGHT ?
			X01_RESULT_CONTINUE : X01_RESULT_PLAYER_OVERSHOOT;
	}

	return X01_RESULT_CONTINUE;
}

