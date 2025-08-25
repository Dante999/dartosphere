#ifndef GAME_GOTCHA_H
#define GAME_GOTCHA_H

#include "player.h"

enum Gotcha_Result {
	GOTCHA_RESULT_CONTINUE,
	GOTCHA_RESULT_PLAYER_WON,
	GOTCHA_RESULT_PLAYER_OVERSHOOT,
	GOTCHA_RESULT_CHECKOUT_NOT_SATISFIED
};

enum Gotcha_Score {
	GOTCHA_SCORE_301,
	GOTCHA_SCORE_501,
	GOTCHA_SCORE_701,
	// ----------
	GOTCHA_SCORE_MAX
};

enum Gotcha_Mode{
	GOTCHA_MODE_STRAIGHT,
	GOTCHA_MODE_DOUBLE,
	GOTCHA_MODE_MASTER,
	// ---------
	GOTCHA_MODE_MAX
};

struct Game_Gotcha{
	enum Gotcha_Score target_score;
	enum Gotcha_Mode  check_in;
	enum Gotcha_Mode  check_out;
};

int         game_gotcha_target_score_as_int(enum Gotcha_Score score);
const char *game_gotcha_target_score_as_string(enum Gotcha_Score score);
const char *game_gotcha_mode_as_string(enum Gotcha_Mode mode);

struct Game_Gotcha *game_gotcha_get_instance(void);
enum Gotcha_Result  game_gotcha_register_dart_throw(struct Player *player);


#endif // GAME_GOTCHA_H
