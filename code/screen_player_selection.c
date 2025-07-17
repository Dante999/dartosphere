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
	CURSOR_MOVING,
	CURSOR_EDITING
} Cursor_State;

static int g_selection_index = 0;
static int g_cursor_state    = CURSOR_MOVING;

static void increase_index(Match *match)
{
	if (g_selection_index < (int)match->player_count+1) {
		g_selection_index++;
	}
}

static void decrease_index(Match *match)
{
	(void) match;

	if (g_selection_index > 0) {
		g_selection_index--;
	}
}

static void screen_show_header(Screen *screen, Match *match)
{
	(void) match;
	const int x = SCREEN_BORDER_WIDTH;
	int       y = Y_OFFSET_HEADER;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "Select Players");
}


#define WIDTH_OPTION_NAME  180
#define WIDTH_OPTION_VALUE  50
#define LINE_INDEX_PLAYER_AMOUNT 0

static void screen_show_player_amount_box(Screen *screen, Match *match)
{
	bool is_selected = (g_selection_index == LINE_INDEX_PLAYER_AMOUNT);

	if (is_selected) {
		if (match->key == DKEY_6) {
			char player_name[255];
			snprintf(player_name, sizeof(player_name), "Player %zu", match->player_count+1);
			match_add_player(match, player_name);
		}
		else if (match->key == DKEY_4) {
			match_remove_player(match);
		}
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE, LINE_INDEX_PLAYER_AMOUNT, is_selected, "Player", "%zu", match->player_count);

}

static void screen_show_players(Screen *screen, Match *match)
{
	for (size_t i=0; i < match->player_count; ++i) {
		Player *player = &match->players[i];

		//const int x = (1+i)*SCREEN_BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int x = SCREEN_BORDER_WIDTH;
		int y = Y_OFFSET_PLAYER_NAMES + (i*(BOX_PLAYER_NAME_HEIGHT+5));

		SDL_Rect outlineRect = {x+TEXT_OFFSET, y+TEXT_OFFSET, BOX_PLAYER_NAME_WIDTH, BOX_PLAYER_NAME_HEIGHT}; // x, y, width, height

		if (g_selection_index == (int)(i+1)) {
			screen_set_color(screen, SCREEN_COLOR_GREY);;
			SDL_RenderFillRect(screen->renderer, &outlineRect);
		}
		screen_set_color(screen, SCREEN_COLOR_BLACK);
		SDL_RenderDrawRect(screen->renderer, &outlineRect);

		screen_draw_text(screen, x, y,  TEXT_FONT_SIZE,  player->name);
	}
}

static void screen_show_continue_button(Screen *screen, Match *match)
{
	const int x = SCREEN_BORDER_WIDTH;
	const int y = Y_OFFSET_CONTINUE_BUTTON;

	SDL_Rect outlineRect = {x+TEXT_OFFSET, y+TEXT_OFFSET, BOX_CONTINUE_BUTTON_WIDTH, BOX_CONTINUE_BUTTON_HEIGHT}; // x, y, width, height

	if (g_selection_index == (int)(match->player_count+1)) {
		screen_set_color(screen, SCREEN_COLOR_GREY);;
		SDL_RenderFillRect(screen->renderer, &outlineRect);
	}
	screen_set_color(screen, SCREEN_COLOR_BLACK);
	SDL_RenderDrawRect(screen->renderer, &outlineRect);

	screen_draw_text(screen, x, y,  FONT_SIZE_CONTINUE_BUTTON,  "continue");
}

void screen_player_selection_on_enter(Screen *screen, Match *match)
{
	(void) screen;
	(void) match;
	g_selection_index = 0;
}
void screen_player_selection_on_exit(Screen *screen, Match *match)
{
	(void) screen;
	(void) match;
}

void screen_player_selection_refresh(Screen *screen, Match *match)
{
	if (match->key == DKEY_MINUS) {
		screen_previous(screen, match);
	}

	if (g_cursor_state == CURSOR_MOVING) {
		if (match->key == DKEY_8)
			decrease_index(match);
		else if (match->key == DKEY_2)
			increase_index(match);

		if (match->key == DKEY_ENTER && g_selection_index == (int)(match->player_count+1)) {
			screen_next(screen, match);
		}
	}

	screen_show_header(screen, match);
	screen_show_player_amount_box(screen, match);
	screen_show_players(screen, match);
	screen_show_continue_button(screen, match);
}

