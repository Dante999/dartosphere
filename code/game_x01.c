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
