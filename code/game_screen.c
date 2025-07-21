#include "game_screen.h"

#include "screen.h"

#include "screen_welcome.h"
#include "screen_player_selection.h"
#include "screen_game_selection.h"


#include <assert.h>

static Game_Screen g_game_screens[] = {
	{
		.id              = GAME_SCREEN_WELCOME,
		.on_enter        = screen_welcome_on_enter,
		.refresh         = screen_welcome_refresh,
		.on_exit         = screen_welcome_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_PLAYERS,
		.previous_screen = GAME_SCREEN_WELCOME
	},
	{
		.id              = GAME_SCREEN_SELECT_PLAYERS,
		.on_enter        = screen_player_selection_on_enter,
		.refresh         = screen_player_selection_refresh,
		.on_exit         = screen_player_selection_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_GAME,
		.previous_screen = GAME_SCREEN_WELCOME
	},
	{
		.id              = GAME_SCREEN_SELECT_GAME,
		.on_enter        = screen_game_selection_on_enter,
		.refresh         = screen_game_selection_refresh,
		.on_exit         = screen_game_selection_on_exit,
		.next_screen     = GAME_SCREEN_WELCOME,
		.previous_screen = GAME_SCREEN_SELECT_PLAYERS
	}
};


static void set_current_screen(struct screen *screen, Game_Screen_Id new_screen)
{
	for (size_t i=0; i < screen->game_screen_list.count; ++i) {
		if (screen->game_screen_list.items[i].id == new_screen) {
			screen->game_screen_list.index_active_screen = i;
		}
	}
}

void game_screen_previous(struct screen *screen, struct match *match)
{
	game_screen_get_current(screen)->on_exit(screen, match);
	set_current_screen(screen, game_screen_get_current(screen)->previous_screen);
	game_screen_get_current(screen)->on_enter(screen, match);
}

void game_screen_next(struct screen *screen, struct match *match)
{
	game_screen_get_current(screen)->on_exit(screen, match);
	set_current_screen(screen, game_screen_get_current(screen)->next_screen);
	game_screen_get_current(screen)->on_enter(screen, match);
}



void game_screen_init(struct screen *screen)
{

}


Game_Screen *game_screen_get_current(struct screen *screen)
{
	Game_Screen_List *list = &screen->game_screen_list;

	assert(list->index_active_screen >= 0);
	assert(list->index_active_screen < list->count-1);

	return &list->items[list->index_active_screen];
}
