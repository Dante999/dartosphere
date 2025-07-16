#include "screen_welcome.h"


void screen_welcome(Screen *screen, Match *match)
{
	(void) match;

	const int x = SCREEN_BORDER_WIDTH;
	const int y = SCREEN_BORDER_WIDTH;

	screen_set_color(screen, SCREEN_COLOR_BLACK);
	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_XL, "Welcome to Dartosphere!");

	if (match->key == DKEY_ENTER) match_next_state(match);
}
