#define RESULT_IMPLEMENTATION
#include "libcutils/result.h"

#define LOGGER_IMPLEMENTATION
#include "libcutils/logger.h"

#include "config.h"
#include "darts.h"
#include "game_screen.h"
#include "screen.h"

#include <unistd.h>


#define SCREEN_SCALE  1.0f
#define SCREEN_WIDTH  800*SCREEN_SCALE
#define SCREEN_HEIGHT 600*SCREEN_SCALE






int main(void)
{
	log_info("Application started!\n");
	config_init();

	struct Match match = {0};
	match_init(&match);

	struct Screen screen = {0};

	game_screen_init(&screen, &match);
	game_screen_get_current(&screen)->on_enter(&screen, &match);

	Result r = screen_init(&screen, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!r.success) {
		log_error("failed to load screen: %s\n", r.msg);
		return 1;
	}


	bool quit = false;

	while(!quit) {
		quit = screen_rendering_start(&screen);
		
		game_screen_get_current(&screen)->refresh(&screen, &match);
		screen_draw_header(&screen);
		screen_draw_status(&screen);

		screen_rendering_stop(&screen);

	}

	screen_destroy(&screen);
}
