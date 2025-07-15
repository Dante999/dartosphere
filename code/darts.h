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
} Game_Mode;


typedef enum {
	GAME_STATE_WELCOME,
	GAME_STATE_SELECT_PLAYERS,
	GAME_STATE_SELECT_GAME,
	GAME_STATE_GAME_ON
} Game_State;

typedef enum {
	DKEY_NONE,
	DKEY_ENTER,
	DKEY_0,
	DKEY_1,
	DKEY_2,
	DKEY_3,
	DKEY_4,
	DKEY_5,
	DKEY_6,
	DKEY_7,
	DKEY_8,
	DKEY_9
} Game_Keypress;

typedef struct {
	Game_Keypress key;
	Game_State state;
	char status_text[255];
	Game_Mode  game;
	Turn   player_turn;
	Player players[MAX_PLAYER_COUNT];
	size_t player_count;
	size_t legs_for_win;
	size_t round;
} Match;

void match_set_state(Match *match, Game_State state);
void match_add_player(Match *match, const char *name);
void match_remove_player(Match *match);


#endif // DARTS_H
