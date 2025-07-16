#include "screen_player_selection.h"

#include "libcutils/logger.h"

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

static int g_line_index = 0;
static int g_selected_game_index = 0;


#define WIDTH_OPTION_NAME  300
#define WIDTH_OPTION_VALUE (SCREEN_LOGICAL_WIDTH-2*SCREEN_BORDER_WIDTH-WIDTH_OPTION_NAME)

static void increase_line_index(Match *match)
{
	(void) match;

	if (g_line_index < LINE_INDEX_MAX-1) {
		g_line_index++;
	}
}

static void decrease_line_index(Match *match)
{
	log_info("decrease line\n");
	(void) match;

	if (g_line_index > 0) {
		g_line_index--;
	}
}

static void screen_show_header(Screen *screen, Match *match)
{
	(void) match;
	const int x = SCREEN_BORDER_WIDTH;
	int       y = Y_OFFSET_HEADER;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "Select Game");
}

//static void screen_show_available_games(Screen *screen, Match *match)
//{
//	int x = SCREEN_BORDER_WIDTH;
//	const int y = Y_OFFSET_PLAYER_COUNT;
//
//	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "Game: ");
//	x += 180;
//
//	SDL_Rect outlineRect = {x+TEXT_OFFSET, y+TEXT_OFFSET, BOX_PLAYER_COUNT_WIDTH, BOX_PLAYER_COUNT_HEIGHT}; // x, y, width, height
//	if (g_selection_index == 0) {
//		screen_set_color(screen, SCREEN_COLOR_GREY);;
//		SDL_RenderFillRect(screen->renderer, &outlineRect);
//
//		if (match->key == DKEY_6) {
//
//			if (g_selection_index < (int)match->available_game_modes.game_modes_count) {
//				++g_selection_index;
//			}
//		}
//		else if (match->key == DKEY_4) {
//			if (g_selection_index > 0) {
//				--g_selection_index;
//			}
//		}
//	}
//
//	screen_set_color(screen, SCREEN_COLOR_BLACK);;
//	SDL_RenderDrawRect(screen->renderer, &outlineRect);
//	screen_draw_text(screen, x, y, TEXT_FONT_SIZE, match->available_game_modes.game_modes[g_selection_index].name);
//}



static void screen_show_available_games(Screen *screen, Match *match)
{
	bool is_selected = (g_line_index == LINE_INDEX_GAME_MODE);

	const int game_index_max = (int)match->available_game_modes.game_modes_count;

	if (is_selected) {
		if (match->key == DKEY_6) {

			if (g_selected_game_index < game_index_max-1) {
				g_selected_game_index++;
			}
			else {
				g_selected_game_index = 0;
			}

		}
		else if (match->key == DKEY_4) {

			if (g_selected_game_index > 0) {
				g_selected_game_index--;
			}
			else {
				g_selected_game_index = game_index_max-1;
			}
		}
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE, LINE_INDEX_GAME_MODE, is_selected, "Game", match->available_game_modes.game_modes[g_selected_game_index].name );
}

static void screen_show_legs_for_win(Screen *screen, Match *match)
{
	bool is_selected = (g_line_index == LINE_INDEX_LEGS_FOR_WIN);

	const size_t legs_max = 10;

	if (is_selected) {
		if (match->key == DKEY_6) {

			if (match->legs_for_win < legs_max) {
				match->legs_for_win++;
			}
		}
		else if (match->key == DKEY_4) {

			if (match->legs_for_win > 2) {
				match->legs_for_win--;
			}
		}
	}

	screen_draw_option(screen, 
			WIDTH_OPTION_NAME, 
			WIDTH_OPTION_VALUE, 
			LINE_INDEX_LEGS_FOR_WIN,
			is_selected, 
			"Legs for win",
			"%zu",
			match->legs_for_win);
}
void screen_game_selection(Screen *screen, Match *match)
{
	if (match->key == DKEY_8) {
		decrease_line_index(match);
	}
	else if (match->key == DKEY_2) {
		increase_line_index(match);
	}
	else if (match->key == DKEY_MINUS) {
		g_line_index = 0;
		match_previous_state(match);
	}
	if (match->key == DKEY_ENTER) {
		g_line_index = 0;
		match_next_state(match);
	}

	screen_show_header(screen, match);
	screen_show_available_games(screen, match);
	screen_show_legs_for_win(screen, match);
}

