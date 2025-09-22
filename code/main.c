#define UTIL_STRINGS_IMPLEMENTATION
#include "libcutils/util_strings.h"

#define RESULT_IMPLEMENTATION
#include "libcutils/result.h"

#define LOGGER_IMPLEMENTATION
#include "libcutils/logger.h"

#include "config.h"
#include "darts.h"
#include "game_screen.h"
#include "screen.h"
#include "sound.h"

#include <unistd.h>
#include <string.h>

#define SCREEN_SCALE  1.0f
#define SCREEN_WIDTH  800*SCREEN_SCALE
#define SCREEN_HEIGHT 600*SCREEN_SCALE




// TODO: signalhandler for graceful closing when hitting CTR+C

int main(int argc, char **argv)
{
	log_info("Application started!\n", NULL);

	char resources_path[255] = "../resources";

	if (argc > 1) {
		strncpy(resources_path, argv[1], sizeof(resources_path));
	}
	Result result = config_init(resources_path);

	if (!result.success) {
		log_error("Failed to load config: %s\n", result.msg);
		return 1;
	}

	struct Match match = {0};
	match_init(&match);

	struct Screen screen = {0};

	game_screen_init(&screen, &match);
	game_screen_get_current(&screen)->on_enter(&screen, &match);

	result = screen_init(&screen, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!result.success) {
		log_error("failed to load screen: %s\n", result.msg);
		return 1;
	}

	result = sound_init();
	if (!result.success) {
		log_error("failed to init sound: %s\n", result.msg);
		return 1;
	}

	while (!screen.quit) {
		screen_rendering_start(&screen);

		game_screen_get_current(&screen)->refresh(&screen, &match);
		screen_draw_header(&screen);
		screen_draw_status(&screen);

		screen_rendering_stop(&screen);

	}

	screen_destroy(&screen);
	sound_destroy();
}
