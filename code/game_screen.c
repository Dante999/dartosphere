#include "game_screen.h"

#include "screen.h"

#include "screen_welcome.h"
#include "screen_player_selection.h"
#include "screen_game_selection.h"
#include "game_gotcha_screen.h"
#include "game_x01_screen.h"
#include "config.h"

#include "libcutils/util_makros.h"
#include "libcutils/logger.h"

#include <SDL.h>
#include <assert.h>

static struct Game_Mode g_game_modes[] = {
	{
		.name = "X01",
		.description = "Start with a given score and reduce it as first",
		.screen_id = GAME_SCREEN_GAME_X01_CONFIGURE
	},
	{
		.name = "Gotcha!",
		.description = "Like X01 but with resetting scores of others",
		.screen_id = GAME_SCREEN_GAME_GOTCHA_CONFIGURE
	}
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
		.id              = GAME_SCREEN_GAME_X01_CONFIGURE,
		.on_enter        = screen_configure_game_x01_on_enter,
		.refresh         = screen_configure_game_x01_refresh,
		.on_exit         = screen_configure_game_x01_on_exit,
		.next_screen     = GAME_SCREEN_GAME_X01_PLAY,
		.previous_screen = GAME_SCREEN_SELECT_GAME,
	},
	{
		.id              = GAME_SCREEN_GAME_X01_PLAY,
		.on_enter        = screen_play_game_x01_on_enter,
		.refresh         = screen_play_game_x01_refresh,
		.on_exit         = screen_play_game_x01_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_PLAYERS,
		.previous_screen = GAME_SCREEN_GAME_X01_CONFIGURE,
	},
	{
		.id              = GAME_SCREEN_GAME_GOTCHA_CONFIGURE,
		.on_enter        = screen_configure_game_gotcha_on_enter,
		.refresh         = screen_configure_game_gotcha_refresh,
		.on_exit         = screen_configure_game_gotcha_on_exit,
		.next_screen     = GAME_SCREEN_GAME_GOTCHA_PLAY,
		.previous_screen = GAME_SCREEN_SELECT_GAME,
	},
	{
		.id              = GAME_SCREEN_GAME_GOTCHA_PLAY,
		.on_enter        = screen_play_game_gotcha_on_enter,
		.refresh         = screen_play_game_gotcha_refresh,
		.on_exit         = screen_play_game_gotcha_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_PLAYERS,
		.previous_screen = GAME_SCREEN_GAME_GOTCHA_CONFIGURE,
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
	strncpy(screen->header.line0, first_line, sizeof(screen->header.line0));
	strncpy(screen->header.line1, second_line, sizeof(screen->header.line1));
	strncpy(screen->header.line2, third_line, sizeof(screen->header.line2));
}

void game_screen_set_status(struct Screen *screen, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vsnprintf(screen->status, sizeof(screen->status), fmt, arg_list);
	va_end(arg_list);
}

#define Y_OFFSET_PLAYER     120
#define PLAYER_BOX_SIZE ((800-(2+MAX_PLAYER_COUNT)*SCREEN_BORDER_WIDTH)/MAX_PLAYER_COUNT)

void game_screen_draw_players(struct Screen *screen, struct Match *match, int (*get_score_of_current_player)(const struct Player *player))
{
	for (size_t i=0; i < match->player_list.count; ++i) {
		struct Player *player = &match->player_list.items[i];

		const int x = (1+i)*SCREEN_BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int y = Y_OFFSET_PLAYER;
		const bool is_selected = ((int) i == match->player_list.index_active_player) ? true : false;

		// if player is selected, get temporary calculated score
		int player_score = is_selected ? get_score_of_current_player(player) : player->score;

		screen_draw_box(screen, x, y, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE, is_selected);

		screen_set_color(screen, SCREEN_COLOR_FONT);
		screen_draw_text(screen, x+10, y+10,  g_config.screen_font_size_m ,  player->name);
		screen_draw_text(screen, x+10, y+50,  g_config.screen_font_size_xl, "%d", player_score);
		screen_draw_text(screen, x+10, y+120, g_config.screen_font_size_xs, "legs: %d", player->legs_won);
	}
}

#define TURN_BOX_WIDTH  150
#define TURN_BOX_HEIGHT  50
#define X_OFFSET_TURN   120
#define Y_OFFSET_TURN   400

void game_screen_draw_turn(struct Screen *screen, struct Match *match)
{
	screen_set_color(screen, SCREEN_COLOR_FONT);
	screen_draw_text(screen, SCREEN_BORDER_WIDTH, Y_OFFSET_TURN, g_config.screen_font_size_l, "Turn: ");

	const int x_start = SCREEN_BORDER_WIDTH + 130;

	struct Turn *turn = &match->player_list.items[match->player_list.index_active_player].turn;

	for (size_t i=0; i < ARRAY_SIZE(turn->dart); ++i) {
		struct Dart_Hit *dart = &turn->dart[i];

		const int x = x_start + (1+i)*SCREEN_BORDER_WIDTH + i*TURN_BOX_WIDTH;
		const int y = Y_OFFSET_TURN;

		bool is_active_throw = ((int) i == turn->index_active_dart);

		char value[50] = {0};
		if (dart->field_value != -1) {
			snprintf(value, sizeof(value), "%c%d",
					field_type_as_char(dart->field_type),
					dart->field_value);
		}
		else {
			snprintf(value, sizeof(value), "%c-",
					field_type_as_char(dart->field_type));
		}

		screen_draw_text_boxed(screen, x, y,
				g_config.screen_font_size_xl, TURN_BOX_WIDTH, is_active_throw,
				"%s", value);
	}

}

