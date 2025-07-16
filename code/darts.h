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
} Field_Type;

char field_type_as_char(Field_Type type);



typedef struct {
	Field_Type field_type;
	int16_t    field_value;
	bool       is_active;
} Dart;

typedef struct {
	Dart dart[3];
} Turn;




typedef enum {
	GAME_STATE_WELCOME,
	GAME_STATE_SELECT_PLAYERS,
	GAME_STATE_SELECT_GAME,
	GAME_STATE_GAME_ON
} Game_State;

typedef enum {
	DKEY_NONE,
	DKEY_ENTER,
	DKEY_MULTIPLY,
	DKEY_DIVIDE,
	DKEY_MINUS,
	DKEY_PLUS,
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
	char name[255];
	char description[255];
} Game_Mode;

typedef struct {
	Game_Mode *game_modes;
	size_t     game_modes_count;
} Available_Game_Modes;




typedef struct {
	char status_text[255];
	Game_Keypress         key;
	Game_State            state;
	Game_Mode            *game_mode;
	Available_Game_Modes available_game_modes;
	Turn          player_turn;
	Player players[MAX_PLAYER_COUNT];
	size_t player_count;
	size_t legs_for_win;
	size_t round;
} Match;

void match_init(Match *match);
void match_previous_state(Match *match);
void match_next_state(Match *match);
void match_add_player(Match *match, const char *name);
void match_remove_player(Match *match);
void match_set_available_game_modes(Match *match, Game_Mode *modes, size_t modes_count);

#endif // DARTS_H
