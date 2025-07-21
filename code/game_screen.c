#include "game_screen.h"

#include "screen.h"

#include "screen_welcome.h"
#include "screen_player_selection.h"
#include "screen_game_selection.h"
#include "screen_game_x01.h"

#include "libcutils/util_makros.h"
#include "libcutils/logger.h"

#include <assert.h>

static struct Game_Mode g_game_modes[] = {
	{
		.name = "X01",
		.description = "Start with a given score and reduce it as first",
		.screen_id = GAME_SCREEN_GAME_X01
	},
	{
		.name = "Around the Clock", 
		.description = "Hit all fields", 
		.screen_id = GAME_SCREEN_UNDEFINED }
};

static struct Game_Screen g_game_screens[] = {
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
	},
	{
		.id              = GAME_SCREEN_GAME_X01,
		.on_enter        = screen_game_x01_on_enter,
		.refresh         = screen_game_x01_refresh,
		.on_exit         = screen_game_x01_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_PLAYERS,
		.previous_screen = GAME_SCREEN_SELECT_GAME,
	}
};


static void set_current_screen(struct Screen *screen, enum Game_Screen_Id new_screen)
{
	for (size_t i=0; i < screen->game_screen_list.count; ++i) {
		if (screen->game_screen_list.items[i].id == new_screen) {
			screen->game_screen_list.index_active_screen = i;
		}
	}
}

void game_screen_previous(struct Screen *screen, struct Match *match)
{
	game_screen_get_current(screen)->on_exit(screen, match);
	set_current_screen(screen, game_screen_get_current(screen)->previous_screen);
	game_screen_get_current(screen)->on_enter(screen, match);
}

void game_screen_next(struct Screen *screen, struct Match *match)
{
	game_screen_get_current(screen)->on_exit(screen, match);
	set_current_screen(screen, game_screen_get_current(screen)->next_screen);
	game_screen_get_current(screen)->on_enter(screen, match);
}



void game_screen_init(struct Screen *screen, struct Match *match)
{
	screen->game_screen_list.index_active_screen = 0;
	screen->game_screen_list.items               = g_game_screens;
	screen->game_screen_list.count               = ARRAY_SIZE(g_game_screens);

	match->game_mode_list.index_active_mode = 0;
	match->game_mode_list.items = g_game_modes;
	match->game_mode_list.count = ARRAY_SIZE(g_game_modes);
}


struct Game_Screen *game_screen_get_current(struct Screen *screen)
{
	struct Game_Screen_List *list = &screen->game_screen_list;

	assert(list->index_active_screen >= 0);
	assert(list->index_active_screen < (int) list->count);

	return &list->items[list->index_active_screen];
}

void game_screen_set_header(struct Screen *screen, const char *first_line, const char *second_line, const char *third_line)
{
	strncpy(screen->header_footer.header_first, first_line, sizeof(screen->header_footer.header_first));
	strncpy(screen->header_footer.header_second, second_line, sizeof(screen->header_footer.header_second));
	strncpy(screen->header_footer.header_third, third_line, sizeof(screen->header_footer.header_third));
}

void game_screen_set_status(struct Screen *screen, const char *status)
{
	strncpy(screen->header_footer.status_text, status, sizeof(screen->header_footer.status_text));
}
