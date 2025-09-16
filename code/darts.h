#ifndef DARTS_H
#define DARTS_H


#define DEFAULT_LEGS_FOR_WIN 3

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "game_screen.h"
#include "player.h"







struct Match {
	struct Player_List    player_list;
	struct Game_Mode_List game_mode_list;
	size_t legs_for_win;
	size_t legs_played;
	size_t round;
};

char field_type_as_char(enum Field_Type type);
void match_init(struct Match *match);


#endif // DARTS_H
