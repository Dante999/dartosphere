#ifndef GAME_X01_H
#define GAME_X01_H

#include "player.h"

enum X01_Result {
	X01_RESULT_CONTINUE,
	X01_RESULT_PLAYER_WON,
	X01_RESULT_PLAYER_OVERSHOOT,
	X01_RESULT_CHECKOUT_NOT_SATISFIED
};

enum X01_Score {
	X01_SCORE_301,
	X01_SCORE_501,
	X01_SCORE_701,
	// ----------
	X01_SCORE_MAX
};

enum X01_Mode{
	X01_MODE_STRAIGHT,
	X01_MODE_DOUBLE,
	X01_MODE_MASTER,
	// ---------
	X01_MODE_MAX
};

struct Game_X01 {
	enum X01_Score start_score;
	enum X01_Mode  check_in;
	enum X01_Mode  check_out;
};

const char *game_x01_score_as_string(enum X01_Score);
const char *game_x01_mode_as_string(enum X01_Mode);

struct Game_X01 *game_x01_get_instance(void);

int game_x01_get_start_score_as_int(struct Game_X01 *game);

enum X01_Result game_x01_register_dart_throw(struct Player *player);


#endif // GAME_X01_H
