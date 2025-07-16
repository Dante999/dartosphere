#ifndef GAME_X01_H
#define GAME_X01_H


typedef enum {
	X01_SCORE_301,
	X01_SCORE_501,
	X01_SCORE_701
} X01_Score;

typedef enum {
	X01_MODE_STRAIGHT,
	X01_MODE_DOUBLE,
	X01_MODE_MASTER
} X01_Mode;

struct {
	X01_Score start_score;
	X01_Mode  mode_in;
	X01_Mode  mode_out;
} Game_X01;


#endif // GAME_X01_H
