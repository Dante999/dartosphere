#ifndef DARTS_H
#define DARTS_H


#define MAX_PLAYER_NAME_LEN 10
#define MAX_PLAYER_COUNT     5
#define DEFAULT_LEGS_FOR_WIN 3

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct {
	char name[MAX_PLAYER_NAME_LEN];
	bool is_active;
	uint16_t score;
	uint16_t legs_won;
} Player;

typedef struct {
	Player items[MAX_PLAYER_COUNT];
	size_t count;
} Player_List;

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

typedef struct {
	char name[255];
	char description[255];
} Game_Mode;

typedef struct {
	Game_Mode *items;
	size_t     count;
} Game_Mode_List;




typedef struct {
	Game_Mode            *game_mode;
	Player_List    player_list;
	Game_Mode_List game_mode_list;
	Turn           player_turn;
	size_t legs_for_win;
	size_t round;
} Match;

void match_init(Match *match);
void match_add_player(Match *match, const char *name);
void match_remove_player(Match *match);
void match_set_game_mode_list(Match *match, Game_Mode *modes, size_t modes_count);


#endif // DARTS_H
