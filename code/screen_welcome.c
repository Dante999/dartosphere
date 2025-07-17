#include "screen_welcome.h"


void screen_welcome_on_enter(Screen *screen, Match *match)
{
	(void) match;

	screen_set_header(screen, "Welcome to Dartosphere!", "", "");
	screen_set_status(screen, "Press <ENTER> to continue!");
}


void screen_welcome_on_exit(Screen *screen, Match *match)
{
	(void) screen;
	(void) match;
}


void screen_welcome_refresh(Screen *screen, Match *match)
{
	if (screen->key_pressed == DKEY_ENTER) screen_next(screen, match);
}
