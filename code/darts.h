#ifndef DARTS_H
#define DARTS_H


#define MAX_PLAYER_NAME_LEN 10
#define MAX_PLAYER_COUNT     5
#define DEFAULT_LEGS_FOR_WIN 3

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct {
	char name[255];
	char description[255];
} Game_Mode;

typedef struct {
	Game_Mode *items;
	size_t     count;
	int        index_active_mode;
} Game_Mode_List;

typedef enum {
	DARTS_SINGLE,
	DARTS_DOUBLE,
	DARTS_TRIPPLE
} Field_Type;

typedef struct {
	Field_Type field_type;
	int16_t    field_value;
} Dart_Hit;

typedef struct {
	Dart_Hit dart[3];
	int index_active_dart;
} Turn;




typedef struct {
	char     name[MAX_PLAYER_NAME_LEN];
	Turn     turn;
	uint16_t score;
	uint16_t legs_won;
} Player;

typedef struct {
	Player items[MAX_PLAYER_COUNT];
	size_t count;
	int    index_active_player;
} Player_List;

typedef struct {
	Player_List    player_list;
	Game_Mode_List game_mode_list;
	size_t legs_for_win;
	size_t round;
} Match;

char field_type_as_char(Field_Type type);
void match_init(Match *match);
void match_add_player(Match *match, const char *name);
void match_remove_player(Match *match);
void match_set_game_mode_list(Match *match, Game_Mode *modes, size_t modes_count);


#endif // DARTS_H
