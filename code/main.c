#define RESULT_IMPLEMENTATION
#include "libcutils/result.h"

#define LOGGER_IMPLEMENTATION
#include "libcutils/logger.h"

#include "darts.h"

#include <string.h>
#include <unistd.h>

#include "screen.h"

#define SCREEN_SCALE  1.0f
#define SCREEN_WIDTH  800*SCREEN_SCALE
#define SCREEN_HEIGHT 600*SCREEN_SCALE


//static Game_Mode g_game_modes[] = {
//	{"X01", "", NULL}
//};




int main(void)
{
	log_info("Application started!\n");

	Match match = {0};
	Screen screen = {0};

	Result r = screen_init(&screen, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!r.success) {
		log_error("failed to load screen: %s\n", r.msg);
		return 1;
	}

	match_set_state(&match, GAME_STATE_WELCOME);

#if 0
	match.game = g_game_modes[0];
	match.legs_for_win = 3;
	match.round        = 11;
	match_add_player(&match, "Gandalf");
	match_add_player(&match, "Frodo");
	match_add_player(&match, "Boromir");
	match_add_player(&match, "Legolas");
	match_add_player(&match, "Aragorn");

	match.players[1].is_active = true;

	match.player_turn.dart[0].multiplicator = DARTS_SINGLE;
	match.player_turn.dart[0].score         = 18;
	match.player_turn.dart[0].is_active     = false;

	match.player_turn.dart[1].multiplicator = DARTS_DOUBLE;
	match.player_turn.dart[1].score         = 20;
	match.player_turn.dart[1].is_active     = false;

	match.player_turn.dart[2].multiplicator = DARTS_DOUBLE;
	match.player_turn.dart[2].score         = -1;
	match.player_turn.dart[2].is_active     = true;
	for( size_t i=0; i < match.player_count; ++i) {
		match.players[i].score = 108*i;
	}
#endif

	bool quit = false;

	while(!quit) {
		quit = screen_refresh(&screen, &match);
	}

	screen_destroy(&screen);
}
