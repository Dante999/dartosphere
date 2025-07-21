#include "screen_player_selection.h"

#include "screen.h"
#include "darts.h"
#include "screen_utils.h"
#include "game_screen.h"


#define Y_OFFSET_HEADER     SCREEN_BORDER_WIDTH
#define Y_OFFSET_PLAYER_COUNT    120
#define Y_OFFSET_PLAYER_NAMES    200
#define Y_OFFSET_CONTINUE_BUTTON 500

#define TEXT_FONT_SIZE    SCREEN_FONT_SIZE_L
#define TEXT_OFFSET                -7
#define BOX_PLAYER_COUNT_HEIGHT   50
#define BOX_PLAYER_COUNT_WIDTH    50
#define BOX_PLAYER_NAME_HEIGHT    50
#define BOX_PLAYER_NAME_WIDTH     400

#define FONT_SIZE_CONTINUE_BUTTON  SCREEN_FONT_SIZE_M
#define BOX_CONTINUE_BUTTON_HEIGHT     30
#define BOX_CONTINUE_BUTTON_WIDTH      130

#define X_OFFSET_INPUT_FIELDS  50




typedef enum {
	LINE_INDEX_GAME_MODE = 0,
	LINE_INDEX_LEGS_FOR_WIN,
	LINE_INDEX_MAX
} Line_Index;

static Line_Cursor g_line_cursor = {
	.index = 0,
	.max_index = LINE_INDEX_MAX-1,
	.cycle = true
};

static Int_Chooser g_chooser_game_mode = {
	.name      = "Game",
	.value     = 0,
	.min_value = 0,
	.max_value = 0, // will be set when on_enter()
	.cycle     = true
};

static Int_Chooser g_chooser_legs_for_win = {
	.name      = "Legs for win",
	.value     = 1,
	.min_value = 1,
	.max_value = 10
};


#define WIDTH_OPTION_NAME  300
#define WIDTH_OPTION_VALUE (SCREEN_LOGICAL_WIDTH-2*SCREEN_BORDER_WIDTH-WIDTH_OPTION_NAME)



static void screen_show_available_games(struct Screen *screen, struct Match *match)
{
	bool is_selected = (g_line_cursor.index == LINE_INDEX_GAME_MODE);

	if (is_selected) {
		if (screen->key_pressed == DKEY_6) {
			int_chooser_increase(&g_chooser_game_mode);
		}
		else if (screen->key_pressed == DKEY_4) {
			int_chooser_decrease(&g_chooser_game_mode);
		}
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE,
		LINE_INDEX_GAME_MODE, is_selected, "Game",
		match->game_mode_list.items[g_chooser_game_mode.value].name );
}

static void screen_show_legs_for_win(struct Screen *screen, struct Match *match)
{
	(void) match;
	bool is_selected = (g_line_cursor.index == LINE_INDEX_LEGS_FOR_WIN);

	if (is_selected) {
		if (screen->key_pressed == DKEY_6) {
			int_chooser_increase(&g_chooser_legs_for_win);
		}
		else if (screen->key_pressed == DKEY_4) {
			int_chooser_decrease(&g_chooser_legs_for_win);
		}
	}

	screen_draw_option(screen,
			WIDTH_OPTION_NAME,
			WIDTH_OPTION_VALUE,
			LINE_INDEX_LEGS_FOR_WIN,
			is_selected,
			"Legs for win",
			"%d",
			g_chooser_legs_for_win.value);
}
void screen_game_selection_on_enter(struct Screen *screen, struct Match *match)
{
	(void) screen;

	g_chooser_legs_for_win.value  = (int) match->legs_for_win;
	g_chooser_game_mode.max_value = match->game_mode_list.count-1;
	g_line_cursor.index           = 0;

	game_screen_set_header(screen, "Select Game Mode", "", "");
	game_screen_set_status(screen, "Press <ENTER> to continue");
}


void screen_game_selection_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;

	match->legs_for_win = (size_t)g_chooser_legs_for_win.value;

	enum Game_Screen_Id next_id = match->game_mode_list.items[g_chooser_game_mode.value].screen_id;
	game_screen_get_current(screen)->next_screen = next_id;
}


void screen_game_selection_refresh(struct Screen *screen, struct Match *match)
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

	screen_show_available_games(screen, match);
	screen_show_legs_for_win(screen, match);
}

