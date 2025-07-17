#define RESULT_IMPLEMENTATION
#include "libcutils/result.h"

#define LOGGER_IMPLEMENTATION
#include "libcutils/logger.h"

#include "libcutils/util_makros.h"
#include "darts.h"

#include <unistd.h>

#include "screen.h"

#define SCREEN_SCALE  1.0f
#define SCREEN_WIDTH  800*SCREEN_SCALE
#define SCREEN_HEIGHT 600*SCREEN_SCALE


static Game_Mode g_game_modes[] = {
	{"X01"             , "Start with a given score and reduce it as first" },
	{"Around the Clock", "Hit all fields" }
};




int main(void)
{
	log_info("Application started!\n");

	Match match = {0};
	match_init(&match);
	match_set_game_mode_list(&match, g_game_modes, ARRAY_SIZE(g_game_modes));
	
	Screen screen = {0};

	Result r = screen_init(&screen, &match, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!r.success) {
		log_error("failed to load screen: %s\n", r.msg);
		return 1;
	}


	bool quit = false;

	while(!quit) {
		quit = screen_refresh(&screen, &match);
	}

	screen_destroy(&screen);
}
