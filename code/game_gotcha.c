#include "game_gotcha.h"

static struct Game_Gotcha g_game = {
	.target_score  = GOTCHA_SCORE_301,
	.check_in     = GOTCHA_MODE_STRAIGHT,
	.check_out    = GOTCHA_MODE_STRAIGHT,
};

struct Game_Gotcha *game_gotcha_get_instance(void)
{
	return &g_game;
}

int game_gotcha_target_score_as_int(enum Gotcha_Score score)
{
	switch(score) {
		case GOTCHA_SCORE_301: return 301;
		case GOTCHA_SCORE_501: return 501;
		case GOTCHA_SCORE_701: return 701;
		default: return 0;
	}
}

const char *game_gotcha_target_score_as_string(enum Gotcha_Score score)
{
	switch(score) {
		case GOTCHA_SCORE_301: return "301";
		case GOTCHA_SCORE_501: return "501";
		case GOTCHA_SCORE_701: return "701";
		default: return "<?>";
	}
}

const char *game_gotcha_mode_as_string(enum Gotcha_Mode mode)
{
	switch(mode) {
		case GOTCHA_MODE_STRAIGHT : return "Straight";
		case GOTCHA_MODE_DOUBLE   : return "Double";
		case GOTCHA_MODE_MASTER   : return "Master";
		default: return "<?>";
	}
}


enum Gotcha_Result game_gotcha_register_dart_throw(struct Player *player)
{
	struct Dart_Hit *hit = player_get_current_dart_throw(player);

	const int score_turn = player_get_score_from_current_turn(player);
	const int player_score = player->score + score_turn;
	const int target_score = game_gotcha_target_score_as_int(g_game.target_score);

	if (player_score > target_score) {
		return GOTCHA_RESULT_PLAYER_OVERSHOOT;
	}
	if (player_score <= target_score-1 && g_game.check_out == GOTCHA_MODE_STRAIGHT) {
		return GOTCHA_RESULT_CONTINUE;
	}
	if (player_score <= target_score-2 && g_game.check_out == GOTCHA_MODE_DOUBLE) {
		return GOTCHA_RESULT_CONTINUE;
	}
	if (player_score <= target_score-3 && g_game.check_out == GOTCHA_MODE_MASTER) {
		return GOTCHA_RESULT_CONTINUE;
	}

	switch(g_game.check_out) {
		case GOTCHA_MODE_STRAIGHT:
			return GOTCHA_RESULT_PLAYER_WON;

		case GOTCHA_MODE_DOUBLE:
			return (hit->field_type == DARTS_DOUBLE)
				? GOTCHA_RESULT_PLAYER_WON : GOTCHA_RESULT_CHECKOUT_NOT_SATISFIED;

		case GOTCHA_MODE_MASTER:
			return (hit->field_type == DARTS_DOUBLE || hit->field_type == DARTS_TRIPPLE)
				? GOTCHA_RESULT_PLAYER_WON : GOTCHA_RESULT_CHECKOUT_NOT_SATISFIED;

		default:
			break;
	}

	return GOTCHA_RESULT_CONTINUE;
}

