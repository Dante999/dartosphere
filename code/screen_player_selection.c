#include "screen_player_selection.h"

#include "config.h"
#include "screen.h"
#include "darts.h"
#include "screen_utils.h"


#define Y_OFFSET_HEADER     SCREEN_BORDER_WIDTH
#define Y_OFFSET_PLAYER_COUNT    120
#define Y_OFFSET_PLAYER_NAMES    200
#define Y_OFFSET_CONTINUE_BUTTON 500

#define TEXT_FONT_SIZE            g_config.screen_font_size_l
#define TEXT_OFFSET                -7
#define BOX_PLAYER_COUNT_HEIGHT   50
#define BOX_PLAYER_COUNT_WIDTH    50
#define BOX_PLAYER_NAME_HEIGHT    50
#define BOX_PLAYER_NAME_WIDTH     400


#define X_OFFSET_INPUT_FIELDS  50

#define WIDTH_OPTION_NAME  180
#define WIDTH_OPTION_VALUE  50
#define LINE_INDEX_PLAYER_AMOUNT 0

typedef enum {
	CURSOR_MOVING,
	CURSOR_EDITING
} Cursor_State;

static Line_Cursor g_line_cursor = {
	.index = 0,
	.max_index = 0,
	.cycle = true
};

static int g_cursor_state = CURSOR_MOVING;

void update_max_line_index(struct Match *match)
{
	g_line_cursor.max_index = match->player_list.count + LINE_INDEX_PLAYER_AMOUNT;
}


static void screen_show_player_amount_box(struct Screen *screen, struct Match *match)
{
	bool is_selected = (g_line_cursor.index == LINE_INDEX_PLAYER_AMOUNT);

	if (is_selected) {
		if (screen->key_pressed == DKEY_6) {
			player_list_add_player(&match->player_list);
		}
		else if (screen->key_pressed == DKEY_4) {
			player_list_remove_player(&match->player_list);
		}

		update_max_line_index(match);
	}

	screen_draw_option(screen, WIDTH_OPTION_NAME, WIDTH_OPTION_VALUE,
		LINE_INDEX_PLAYER_AMOUNT, is_selected,
		"Player", "%zu", match->player_list.count);

}

static void screen_show_players(struct Screen *screen, struct Match *match)
{
	for (size_t i=0; i < match->player_list.count; ++i) {
		const struct Player *player = &match->player_list.items[i];

		//const int x = (1+i)*SCREEN_BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int x = SCREEN_BORDER_WIDTH*2;
		int y = Y_OFFSET_PLAYER_NAMES + (i*(BOX_PLAYER_NAME_HEIGHT+10));

		bool is_selected = (g_line_cursor.index == (int)(i+LINE_INDEX_PLAYER_AMOUNT+1));

		screen_draw_text_boxed(screen, x, y, TEXT_FONT_SIZE, BOX_PLAYER_NAME_WIDTH, is_selected, player->name);
	}
}


void screen_player_selection_on_enter(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
	g_line_cursor.index = 0;
	update_max_line_index(match);

	game_screen_set_header(screen, "Select Players", "", "");
	game_screen_set_status(screen, "Press <ENTER> to continue");
}
void screen_player_selection_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
}

void screen_player_selection_refresh(struct Screen *screen, struct Match *match)
{
	if (screen->key_pressed == DKEY_MINUS) {
		game_screen_previous(screen, match);
	}

	if (g_cursor_state == CURSOR_MOVING) {
		if (screen->key_pressed == DKEY_8)
			line_cursor_up(&g_line_cursor);
		else if (screen->key_pressed == DKEY_2)
			line_cursor_down(&g_line_cursor);

		if (screen->key_pressed == DKEY_ENTER) {
			game_screen_next(screen, match);
		}
	}

	screen_show_player_amount_box(screen, match);
	screen_show_players(screen, match);
}

