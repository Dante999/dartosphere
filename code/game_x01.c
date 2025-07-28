#include "game_x01.h"

static struct Game_X01 g_game = {
	.start_score  = X01_SCORE_301,
	.check_in     = X01_MODE_STRAIGHT,
	.check_out    = X01_MODE_STRAIGHT,
};

struct Game_X01 *game_x01_get_instance(void)
{
	return &g_game;
}

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


enum X01_Result game_x01_register_dart_throw(struct Player *player)
{
	struct Dart_Hit *hit = player_get_current_dart_throw(player);
	
	int hit_score = dart_hit_get_points(hit);
	int score_delta = player->score - hit_score;

	if (score_delta < 0) {
		return X01_RESULT_PLAYER_OVERSHOOT;
	}
	if (score_delta > 0) {
		return X01_RESULT_CONTINUE;
	}

	switch(g_game.check_out) {
		case X01_MODE_STRAIGHT:
			return X01_RESULT_PLAYER_WON;

		case X01_MODE_DOUBLE:
			return (hit->field_type == DARTS_DOUBLE) 
				? X01_RESULT_PLAYER_WON : X01_RESULT_CHECKOUT_NOT_SATISFIED;

		case X01_MODE_MASTER:
			return (hit->field_type == DARTS_DOUBLE || hit->field_type == DARTS_TRIPPLE) 
				? X01_RESULT_PLAYER_WON : X01_RESULT_CHECKOUT_NOT_SATISFIED;

		default:
			break;
	}

	return X01_RESULT_CONTINUE;
}

