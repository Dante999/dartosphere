#include "game_x01_screen.h"

#include "darts.h"
#include "game_x01.h"
#include "player.h"
#include "screen.h"
#include "screen_utils.h"

#include "libcutils/util_makros.h"


enum Game_Status {
	GAME_STATUS_CONFIGURING,
	GAME_STATUS_PLAYING
};

static enum Game_Status g_status = GAME_STATUS_CONFIGURING;


#define Y_OFFSET_HEADER     SCREEN_BORDER_WIDTH
#define Y_OFFSET_PLAYER     120
#define Y_OFFSET_TURN       400

#define PLAYER_BOX_SIZE ((800-(2+MAX_PLAYER_COUNT)*SCREEN_BORDER_WIDTH)/MAX_PLAYER_COUNT)

#define TURN_BOX_WIDTH   100
#define TURN_BOX_HEIGHT  50


static void screen_show_players(struct Screen *screen, struct Match *match)
{
	for (size_t i=0; i < match->player_list.count; ++i) {
		struct Player *player = &match->player_list.items[i];

		const int x = (1+i)*SCREEN_BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int y = Y_OFFSET_PLAYER;
		int player_score = player->score;

		SDL_Rect outlineRect = {x, y, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE}; // x, y, width, height
		if ((int) i == match->player_list.index_active_player) {
			screen_set_color(screen, SCREEN_COLOR_GREY);;
			SDL_RenderFillRect(screen->renderer, &outlineRect);
			player_score -= player_get_score_from_current_turn(player);
		}
		screen_set_color(screen, SCREEN_COLOR_BLACK);
		SDL_RenderDrawRect(screen->renderer, &outlineRect);

		screen_draw_text(screen, x+10, y+10,  SCREEN_FONT_SIZE_S ,  player->name);
		screen_draw_text(screen, x+10, y+50,  SCREEN_FONT_SIZE_XL, "%d", player_score);
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

	struct Turn *turn = &match->player_list.items[match->player_list.index_active_player].turn;

	for (size_t i=0; i < ARRAY_SIZE(turn->dart); ++i) {
		struct Dart_Hit *dart = &turn->dart[i];

		const int x = x_start + (1+i)*SCREEN_BORDER_WIDTH + i*TURN_BOX_WIDTH;
		const int y = Y_OFFSET_TURN;


		SDL_Rect outlineRect = {x-10, y-8, TURN_BOX_WIDTH, TURN_BOX_HEIGHT}; // x, y, width, height

		if ((int) i == turn->index_active_dart) {
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

	}

}

static void playing_set_header(struct Screen *screen, struct Match *match)
{
	struct Game_X01 *game = game_x01_get_instance();
	struct Screen_Header *hf =&screen->header;

	snprintf(hf->line0, sizeof(hf->line0), "Game: X01");
	snprintf(hf->line1, sizeof(hf->line1),
			"score: %s    check_in: %s    check_out: %s",
			game_x01_score_as_string(game->start_score),
			game_x01_mode_as_string(game->check_in),
			game_x01_mode_as_string(game->check_out));
	snprintf(hf->line2, sizeof(hf->line2), "Round: %zu    Legs for win: %zu",
			match->round, match->legs_for_win);
}

static void playing_screen_next_step(struct Screen *screen, struct Match *match)
{
	struct Player *active_player = player_list_get_active_player(&match->player_list);

	if (!dart_hit_is_valid_input(player_get_current_dart_throw(active_player))) {
		game_screen_set_status(screen, "Entered value is not valid!");
		return;
	}

	enum X01_Result res = game_x01_register_dart_throw(active_player);

	switch (res) {
		case X01_RESULT_CHECKOUT_NOT_SATISFIED: // fallthrough
		case X01_RESULT_PLAYER_OVERSHOOT:
			player_clear_dart_throws(active_player);
			player_list_select_next(&match->player_list);
			active_player = player_list_get_active_player(&match->player_list);
			player_clear_dart_throws(active_player);
			return;
			break;
		case X01_RESULT_PLAYER_WON:
			// TODO
			break;
		case X01_RESULT_CONTINUE:


	}


	bool has_another_throw = player_next_dart_throw(active_player);

	if (!has_another_throw) {
		int score = player_get_score_from_current_turn(active_player);

		active_player->score -= score;

		player_list_select_next(&match->player_list);
		active_player = player_list_get_active_player(&match->player_list);
		player_clear_dart_throws(active_player);
	}
}

static void playing_undo_turn(struct Match *match)
{
	struct Player *active_player = player_list_get_active_player(&match->player_list);

	struct Dart_Hit *active_turn = player_get_current_dart_throw(active_player);

	if (active_turn->field_value > 0) {
		active_turn->field_value /= 10;
		return;
	}

	bool has_another_throw = player_previous_dart_throw(active_player);

	if (!has_another_throw) {
		// TODO: handle downcounting of already added scores from this
		// round
		player_list_select_previous(&match->player_list);
		active_player = player_list_get_active_player(&match->player_list);
	}
}

static void handle_score_input(struct Screen *screen, struct Match *match)
{
	struct Player *active_player = player_list_get_active_player(&match->player_list);
	struct Dart_Hit *current_throw = player_get_current_dart_throw(active_player);

	if (screen->key_pressed == DKEY_DIVIDE) {
		dart_hit_toggle_field_type(current_throw, DARTS_DOUBLE);
	}
	else if (screen->key_pressed == DKEY_MULTIPLY) {
		dart_hit_toggle_field_type(current_throw, DARTS_TRIPPLE);
	}
	else {
		int key_val = screen_key_numeric_value(screen->key_pressed);

		if (key_val < 0) return;

		if (current_throw->field_value < 0) {
			current_throw->field_value = key_val;
		}
		else if (current_throw->field_value < 10) {
			current_throw->field_value *= 10;
			current_throw->field_value += key_val;
		}
	}
}

void screen_play_game_x01_refresh(struct Screen *screen, struct Match *match)
{
	if (screen->key_pressed == DKEY_MINUS) {
		playing_undo_turn(match);
	}
	else if (screen->key_pressed == DKEY_ENTER) {
		playing_screen_next_step(screen, match);
	}
	else {
		handle_score_input(screen, match);
	}

	playing_set_header(screen, match);
	screen_show_players(screen, match);
	screen_show_turn(screen, match);
}

void screen_play_game_x01_on_enter(struct Screen *screen, struct Match *match)
{
	struct Game_X01 *game = game_x01_get_instance();

	for( size_t i=0; i < match->player_list.count; ++i) {
		struct Player *player = &match->player_list.items[i];
		player->score = game_x01_get_start_score_as_int(game);
		player_clear_dart_throws(player);
	}

	game_screen_set_status(screen, "Game On!");
}

void screen_play_game_x01_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
}

/******************************************************************************
 * CONFIGURATION SCREEN
 ******************************************************************************/
#define WIDTH_OPTION_NAME  300
#define WIDTH_OPTION_VALUE (SCREEN_LOGICAL_WIDTH-2*SCREEN_BORDER_WIDTH-WIDTH_OPTION_NAME)



typedef enum {
	LINE_INDEX_SCORE = 0,
	LINE_INDEX_CHECK_IN,
	LINE_INDEX_CHECK_OUT,
	// -----------
	LINE_INDEX_MAX
} Line_Index;


static const char *score_to_string(int value)
{
	return game_x01_score_as_string(value);
}

static const char *mode_to_string(int value)
{
	return game_x01_mode_as_string(value);
}

static Int_Chooser g_choosers[] = {
	{
		.index        = LINE_INDEX_SCORE,
		.name         = "Start Score",
		.value        = 0,
		.min_value    = 0,
		.max_value    = X01_SCORE_MAX-1,
		.cycle        = true,
		.key_increase = DKEY_6,
		.key_decrease = DKEY_4,
		.value_to_string = score_to_string
	},
	{
		.index        = LINE_INDEX_CHECK_IN,
		.name         = "Check In",
		.value        = 0,
		.min_value    = 0,
		.max_value    = X01_MODE_MAX-1,
		.cycle        = true,
		.key_increase = DKEY_6,
		.key_decrease = DKEY_4,
		.value_to_string = mode_to_string
	},
	{
		.index        = LINE_INDEX_CHECK_OUT,
		.name         = "Check Out",
		.value        = 0,
		.min_value    = 0,
		.max_value    = X01_MODE_MAX-1,
		.cycle        = true,
		.key_increase = DKEY_6,
		.key_decrease = DKEY_4,
		.value_to_string = mode_to_string
	}
};


static struct Chooser_Bundle g_chooser_bundle = {
	.width_chooser_name  = WIDTH_OPTION_NAME,
	.width_chooser_value = WIDTH_OPTION_VALUE,
	.cursor = {
		.index     = 0,
		.max_index = LINE_INDEX_MAX-1,
		.cycle     = true
	},
	.chooser_list  = g_choosers,
	.chooser_count = ARRAY_SIZE(g_choosers)
};


void screen_configure_game_x01_refresh(struct Screen *screen, struct Match *match)
{
	if (screen->key_pressed == DKEY_8) {
		line_cursor_up(&g_chooser_bundle.cursor);
	}
	else if (screen->key_pressed == DKEY_2) {
		line_cursor_down(&g_chooser_bundle.cursor);
	}
	else if (screen->key_pressed == DKEY_MINUS) {
		game_screen_previous(screen, match);
	}
	if (screen->key_pressed == DKEY_ENTER) {
		game_screen_next(screen, match);
	}

	game_screen_set_header(screen, "X01: Configuration", "", "");
	update_and_draw_chooser(screen, &g_chooser_bundle);

	if (screen->key_pressed == DKEY_ENTER) g_status = GAME_STATUS_PLAYING;

}

void screen_configure_game_x01_on_enter(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
	struct Game_X01 *game = game_x01_get_instance();

	get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_SCORE)->value     = game->start_score;
	get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_IN)->value  = game->check_in;
	get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_OUT)->value = game->check_out;
}

void screen_configure_game_x01_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
	struct Game_X01 *game = game_x01_get_instance();

	game->start_score = get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_SCORE)->value;
	game->check_in    = get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_IN)->value;
	game->check_out   = get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_OUT)->value;
}


