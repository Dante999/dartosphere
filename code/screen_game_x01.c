#include "screen_game_x01.h"

#include "game_x01.h"
#include "screen.h"
#include "screen_utils.h"
#include "darts.h"

#include "libcutils/util_makros.h"


// TODO: Move into separate translation unit
#if 0

#define Y_OFFSET_HEADER     SCREEN_BORDER_WIDTH
#define Y_OFFSET_PLAYER     120
#define Y_OFFSET_TURN       400

#define PLAYER_BOX_SIZE ((800-(2+MAX_PLAYER_COUNT)*SCREEN_BORDER_WIDTH)/MAX_PLAYER_COUNT)

#define TURN_BOX_WIDTH   100
#define TURN_BOX_HEIGHT  50


static void screen_show_players(Screen *screen, Match *match)
{
	for (size_t i=0; i < match->player_count; ++i) {
		Player *player = &match->players[i];

		const int x = (1+i)*SCREEN_BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int y = Y_OFFSET_PLAYER;

		SDL_Rect outlineRect = {x, y, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE}; // x, y, width, height
		if (player->is_active) {
			screen_set_color(screen, SCREEN_COLOR_GREY);;
			SDL_RenderFillRect(screen->renderer, &outlineRect);
		}
		screen_set_color(screen, SCREEN_COLOR_BLACK);
		SDL_RenderDrawRect(screen->renderer, &outlineRect);

		screen_draw_text(screen, x+10, y+10,  SCREEN_FONT_SIZE_S ,  player->name);
		screen_draw_text(screen, x+10, y+50,  SCREEN_FONT_SIZE_XL, "%d", player->score);
		screen_draw_text(screen, x+10, y+120, SCREEN_FONT_SIZE_XS, "legs: %d", player->legs_won);
	}
}

#define X_OFFSET_TURN   120
#define TURN_Y_TEXT_OFFSET 10

static void screen_show_turn(struct Screen *screen, struct Match *match)
{
	screen_set_color(screen, SCREEN_COLOR_BLACK);
	screen_draw_text(screen, SCREEN_BORDER_WIDTH, Y_OFFSET_TURN, SCREEN_FONT_SIZE_L, "Turn: ");

	const int x_start = SCREEN_BORDER_WIDTH + 130;

	for (size_t i=0; i < ARRAY_SIZE(match->player_turn.dart); ++i) {
		Dart *dart = &match->player_turn.dart[i];

		const int x = x_start + (1+i)*SCREEN_BORDER_WIDTH + i*TURN_BOX_WIDTH;
		const int y = Y_OFFSET_TURN;


		SDL_Rect outlineRect = {x-10, y-8, TURN_BOX_WIDTH, TURN_BOX_HEIGHT}; // x, y, width, height

		if (dart->is_active) {
			screen_set_color(screen, SCREEN_COLOR_GREY);
			SDL_RenderFillRect(screen->renderer, &outlineRect);
		}

		screen_set_color(screen, SCREEN_COLOR_BLACK);
		SDL_RenderDrawRect(screen->renderer, &outlineRect);

		if (dart->field_value != -1) {
			screen_draw_text(screen, x, y-3, SCREEN_FONT_SIZE_XL, "%c%d",
				field_type_as_char(dart->field_type),
				dart->field_value);
		}
		else if (dart->field_value == 0) {
			screen_draw_text(screen, x, y-3, SCREEN_FONT_SIZE_XL, "MISS");
		}

	}

}
#endif 


/******************************************************************************
 * CONFIGURATION SCREEN
 ******************************************************************************/
#define WIDTH_OPTION_NAME  300
#define WIDTH_OPTION_VALUE (SCREEN_LOGICAL_WIDTH-2*SCREEN_BORDER_WIDTH-WIDTH_OPTION_NAME)

//enum Game_Status {
//	GAME_STATUS_CONFIGURING,
//	GAME_STATUS_PLAYING
//};
//
//static enum Game_Status g_status = GAME_STATUS_CONFIGURING;

static struct Game_X01 g_game = {
	.start_score  = X01_SCORE_301,
	.check_in     = X01_MODE_STRAIGHT,
	.check_out    = X01_MODE_STRAIGHT,
};

typedef enum {
	LINE_INDEX_SCORE = 0,
	LINE_INDEX_CHECK_IN,
	LINE_INDEX_CHECK_OUT,
	// -----------
	LINE_INDEX_MAX
} Line_Index;

static Line_Cursor g_line_cursor = {
	.index = 0,
	.max_index = LINE_INDEX_MAX-1,
	.cycle = true
};

static Int_Chooser g_chooser_score = {
	.name      = "Start Score",
	.value     = 0,
	.min_value = 0,
	.max_value = X01_SCORE_MAX-1,
	.cycle     = true
};

static Int_Chooser g_chooser_check_in = {
	.name      = "Check In",
	.value     = 0,
	.min_value = 0,
	.max_value = X01_MODE_MAX-1,
	.cycle     = true
};

static Int_Chooser g_chooser_check_out = {
	.name      = "Check Out",
	.value     = 0,
	.min_value = 0,
	.max_value = X01_MODE_MAX-1,
	.cycle     = true
};


static void show_option_score(struct Screen *screen) {
	bool is_selected = (g_line_cursor.index == LINE_INDEX_SCORE);

	if (is_selected) {
		if (screen->key_pressed == DKEY_6) {
			int_chooser_increase(&g_chooser_score);
		}
		else if (screen->key_pressed == DKEY_4) {
			int_chooser_decrease(&g_chooser_score);
		}
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE,
			LINE_INDEX_SCORE, is_selected, "Score",
			game_x01_score_as_string(g_chooser_score.value));
}

static void show_option_check_in(struct Screen *screen) {
	bool is_selected = (g_line_cursor.index == LINE_INDEX_CHECK_IN);

	if (is_selected) {
		if (screen->key_pressed == DKEY_6) {
			int_chooser_increase(&g_chooser_check_in);
		}
		else if (screen->key_pressed == DKEY_4) {
			int_chooser_decrease(&g_chooser_check_in);
		}
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE,
			LINE_INDEX_CHECK_IN, is_selected, "Check In",
			game_x01_mode_as_string(g_chooser_check_in.value));
}

// TODO: factor out line index, and keys for increasing/decreasing of the 
// chooser. Probably store it in a struct and just call helper function to 
// reduce boiler plate code
static void show_option_check_out(struct Screen *screen) {
	bool is_selected = (g_line_cursor.index == LINE_INDEX_CHECK_OUT);

	if (is_selected) {
		if (screen->key_pressed == DKEY_6) {
			int_chooser_increase(&g_chooser_check_out);
		}
		else if (screen->key_pressed == DKEY_4) {
			int_chooser_decrease(&g_chooser_check_out);
		}
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE,
			LINE_INDEX_CHECK_OUT, is_selected, "Check Out",
			game_x01_mode_as_string(g_chooser_check_out.value));
}

void screen_game_x01_refresh(struct Screen *screen, struct Match *match)
{
	if (screen->key_pressed == DKEY_8) {
		line_cursor_up(&g_line_cursor);
	}
	else if (screen->key_pressed == DKEY_2) {
		line_cursor_down(&g_line_cursor);
	}
	else if (screen->key_pressed == DKEY_MINUS) {
		game_screen_previous(screen, match);
	}
	if (screen->key_pressed == DKEY_ENTER) {
		game_screen_next(screen, match);
	}

	game_screen_set_header(screen, "X01: Configuration", "", "");
	show_option_score(screen);
	show_option_check_in(screen);
	show_option_check_out(screen);

	if (screen->key_pressed == DKEY_ENTER) game_screen_next(screen, match);
	
}

void screen_game_x01_on_enter(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;

	g_game.start_score = g_chooser_score.value;
	g_game.check_in    = g_chooser_check_in.value;
	g_game.check_out   = g_chooser_check_out.value;
}

void screen_game_x01_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
	
	g_chooser_score.value     = g_game.start_score;
	g_chooser_check_in.value  = g_game.check_in;
	g_chooser_check_out.value = g_game.check_out;
}

