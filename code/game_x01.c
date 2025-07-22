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
