#define RESULT_IMPLEMENTATION
#include "libcutils/result.h"

#define LOGGER_IMPLEMENTATION
#include "libcutils/logger.h"

#include "darts.h"

#include <string.h>
#include <unistd.h>

#include "ui_sdl.h"

#define SCREEN_SCALE  1.0f
#define SCREEN_WIDTH  800*SCREEN_SCALE
#define SCREEN_HEIGHT 600*SCREEN_SCALE



void game_add_player(Game *game, const char *player_name)
{
	if (game->player_count < MAX_PLAYER_COUNT) {
		strncpy(game->players[game->player_count].name, player_name, MAX_PLAYER_NAME_LEN);
		game->player_count++;
	}
}


int main(void)
{
	log_info("Application started!\n");

	Game game = {0};
	Darts_Gui ui = {
		.init = ui_sdl_init,
		.refresh = ui_sdl_refresh,
		.destroy = ui_sdl_destroy
	};

	Result r = ui.init(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!r.success) {
		//log_error("failed to load ui: %s\n", r.msg);
		return 1;
	}

	game_add_player(&game, "Gandalf");
	game_add_player(&game, "Frodo");
	game_add_player(&game, "Boromir");
	game_add_player(&game, "Legolas");
	game_add_player(&game, "Aragorn");

	game.players[1].is_active = true;
	
	game.player_turn.dart[0].multiplicator = DARTS_SINGLE;
	game.player_turn.dart[0].score         = 18;
	game.player_turn.dart[0].is_active     = false;

	game.player_turn.dart[1].multiplicator = DARTS_DOUBLE;
	game.player_turn.dart[1].score         = 20;
	game.player_turn.dart[1].is_active     = false;

	game.player_turn.dart[2].multiplicator = DARTS_DOUBLE;
	game.player_turn.dart[2].score         = -1;
	game.player_turn.dart[2].is_active     = true;

	for( size_t i=0; i < game.player_count; ++i) {
		game.players[i].score = 108*i;
	}

	bool quit = false;

	while(!quit) {
		quit = ui.refresh(&game);
	}

	ui.destroy();
}
