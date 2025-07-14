#ifndef DARTS_H
#define DARTS_H


#define MAX_PLAYER_NAME_LEN 10
#define MAX_PLAYER_COUNT     5

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct {
	char name[MAX_PLAYER_NAME_LEN];
	bool is_active;
	uint16_t score;
	uint16_t legs_won;
} Player;


typedef enum {
	DARTS_SINGLE,
	DARTS_DOUBLE,
	DARTS_TRIPPLE
} Multiplicator;

char multiplicator_as_char(Multiplicator m);



typedef struct {
	Multiplicator multiplicator;
	int16_t score;
	bool is_active;
} Dart;

typedef struct {
	Dart dart[3];
} Turn;


typedef struct {
	char name[255];
	char modifiers[255];
	void *data;
} Game;

typedef struct {
	Game   *game;
	Turn   player_turn;
	Player players[MAX_PLAYER_COUNT];
	size_t player_count;
	size_t legs_for_win;
	size_t round;
} Match;


#endif // DARTS_H
