#include "screen_welcome.h"


void screen_welcome_on_enter(struct Screen *screen, struct Match *match)
{
	(void) match;

	game_screen_set_header(screen, "Welcome to Dartosphere!", "", "");
	game_screen_set_status(screen, "Press <ENTER> to continue!");
}


void screen_welcome_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
}


void screen_welcome_refresh(struct Screen *screen, struct Match *match)
{
	if (screen->key_pressed == DKEY_ENTER) game_screen_next(screen, match);
}
