#include "game_gotcha_screen.h"

#include "config.h"
#include "darts.h"
#include "game_gotcha.h"
#include "player.h"
#include "screen.h"
#include "screen_utils.h"
#include "sound.h"

#include "libcutils/util_makros.h"
#include "libcutils/logger.h"

// TODO: set other player score to zero if same score is reached
//       problem: everything must be temporary for that round in case wrong
//       value was entered and player does trigger undo

enum Game_Status {
	GAME_STATUS_PLAYING,
	GAME_STATUS_PLAYER_WON_LEG,
	GAME_STATUS_PLAYER_WON_MATCH
};

static enum Game_Status g_status = GAME_STATUS_PLAYING;
static int g_index_first_player  = -1;

static int get_temp_score_of_current_player(const struct Player *player)
{
	return player->score + player_get_score_from_current_turn(player);
}

static void playing_set_header(struct Screen *screen, struct Match *match)
{
	struct Game_Gotcha *game = game_gotcha_get_instance();
	struct Screen_Header *hf =&screen->header;

	snprintf(hf->line0, sizeof(hf->line0), "Game: Gotcha!");
	snprintf(hf->line1, sizeof(hf->line1),
			"target_score: %s    check_in: %s    check_out: %s",
			game_gotcha_target_score_as_string(game->target_score),
			game_gotcha_mode_as_string(game->check_in),
			game_gotcha_mode_as_string(game->check_out));
	snprintf(hf->line2, sizeof(hf->line2), "Round: %zu    Legs played: %zu    Legs for win: %zu",
			match->round, match->legs_played, match->legs_for_win);
}

static void playing_screen_next_step(struct Screen *screen, struct Match *match)
{
	struct Player *active_player = player_list_get_active_player(&match->player_list);

	if (!dart_hit_is_valid_input(player_get_current_dart_throw(active_player))) {
		game_screen_set_status(screen, "Entered value is not valid!");
		return;
	}
	else {
		sound_play_score(player_get_current_dart_throw(active_player));
		game_screen_set_status(screen, "");
	}

	enum Gotcha_Result res = game_gotcha_register_dart_throw(active_player);

	switch (res) {
		case GOTCHA_RESULT_CHECKOUT_NOT_SATISFIED: // fallthrough
			log_info("%s finished but checkout is not satisfied!\n", active_player->name);
			game_screen_set_status(screen, "%s: checkout rule not satisfied!", active_player->name);
			player_clear_dart_throws(active_player);
			player_list_select_next(&match->player_list);
			active_player = player_list_get_active_player(&match->player_list);
			player_clear_dart_throws(active_player);
			return;
		case GOTCHA_RESULT_PLAYER_OVERSHOOT:
			log_info("%s did overshoot!\n", active_player->name);
			game_screen_set_status(screen, "%s overshoot!", active_player->name);
			player_clear_dart_throws(active_player);
			player_list_select_next(&match->player_list);
			active_player = player_list_get_active_player(&match->player_list);
			player_clear_dart_throws(active_player);
			return;
		case GOTCHA_RESULT_PLAYER_WON:
			log_info("%s won the leg!\n", active_player->name);
			game_screen_set_status(screen, "%s won!", active_player->name);
			active_player->legs_won++;
			g_status = GAME_STATUS_PLAYER_WON_LEG;
			return;
		case GOTCHA_RESULT_CONTINUE:
			break;
	}


	bool has_another_throw = player_next_dart_throw(active_player);

	if (has_another_throw) {
		log_info("%s has another throw\n", active_player->name);
	}
	else {
		log_info("%s finished his turn\n", active_player->name);
		int score = player_get_score_from_current_turn(active_player);

		active_player->score += score;

		player_list_select_next(&match->player_list);
		active_player = player_list_get_active_player(&match->player_list);
		player_clear_dart_throws(active_player);
		log_info("%s starts his turn\n", active_player->name);

		if (match->player_list.index_active_player == g_index_first_player) {
			match->round++;
		}
	}
}

static void playing_undo_turn(struct Match *match)
{
	struct Player *active_player = player_list_get_active_player(&match->player_list);

	struct Dart_Hit *active_turn = player_get_current_dart_throw(active_player);

	if (active_turn->field_value > 0) {
		active_turn->field_value /= 10;

		if (active_turn->field_value == 0) {
			active_turn->field_value = DART_FIELD_VALUE_UNSET;
		}
		return;
	}

	bool has_another_throw = player_previous_dart_throw(active_player);

	if (!has_another_throw) {
		// TODO: handle downcounting of already added scores from this
		// round
		player_list_select_previous(&match->player_list);
		active_player = player_list_get_active_player(&match->player_list);

		if (match->player_list.index_active_player == g_index_first_player &&
			match->round > 0) {
			match->round--;
		}
	}
}

static void handle_score_input(struct Screen *screen, struct Match *match)
{
	struct Player   *active_player = player_list_get_active_player(&match->player_list);
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

static void screen_play_game_gotcha_player_won_match(struct Screen *screen, struct Match *match)
{
	playing_set_header(screen, match);

	struct Player *player = player_list_get_active_player(&match->player_list);

	screen_draw_text_boxed(screen, SCREEN_BORDER_WIDTH, SCREEN_LOGICAL_HEIGHT/2, g_config.screen_font_size_xl, 0,
			true, "%s won this match!", player->name);

	if (screen->key_pressed == DKEY_ENTER) {
		game_screen_previous(screen, match);
	}
}

static void screen_play_game_gotcha_player_won_leg(struct Screen *screen, struct Match *match)
{
	playing_set_header(screen, match);

	struct Player *player = player_list_get_active_player(&match->player_list);

	screen_draw_text_boxed(screen, SCREEN_BORDER_WIDTH, SCREEN_LOGICAL_HEIGHT/2, g_config.screen_font_size_xl, 0,
			true, "%s won this leg!", player->name);

	if (screen->key_pressed == DKEY_ENTER) {

		if (player->legs_won == match->legs_for_win) {
			g_status = GAME_STATUS_PLAYER_WON_MATCH;
		}
		else {
			match->legs_played++;
			log_info("starting new leg\n", NULL);
			g_status = GAME_STATUS_PLAYING;
			screen_play_game_gotcha_on_enter(screen, match);
		}
	}
}

static void screen_play_game_gotcha_game_on(struct Screen *screen, struct Match *match)
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
	game_screen_draw_players(screen, match, get_temp_score_of_current_player);
	game_screen_draw_turn(screen, match);
}

void screen_play_game_gotcha_refresh(struct Screen *screen, struct Match *match)
{
	switch(g_status) {
		case GAME_STATUS_PLAYING:
			screen_play_game_gotcha_game_on(screen, match);
			break;
		case GAME_STATUS_PLAYER_WON_LEG:
			screen_play_game_gotcha_player_won_leg(screen, match);
			break;
		case GAME_STATUS_PLAYER_WON_MATCH:
			screen_play_game_gotcha_player_won_match(screen, match);
			break;


	}
}

void screen_play_game_gotcha_on_enter(struct Screen *screen, struct Match *match)
{
	sound_play_effect(SOUND_EFFECT_GAME_ON);

	for( size_t i=0; i < match->player_list.count; ++i) {
		struct Player *player = &match->player_list.items[i];
		player->score = 0;
		player_clear_dart_throws(player);
	}

	g_status = GAME_STATUS_PLAYING;
	log_info("old index of first player: %d\n", g_index_first_player);
	g_index_first_player++;
	g_index_first_player %= match->player_list.count;
	log_info("new index of first player: %d\n", g_index_first_player);
	match->player_list.index_active_player = g_index_first_player;
	match->round = 0;
	game_screen_set_status(screen, "Game On!");
}

void screen_play_game_gotcha_on_exit(struct Screen *screen, struct Match *match)
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
	return game_gotcha_target_score_as_string(value);
}

static const char *mode_to_string(int value)
{
	return game_gotcha_mode_as_string(value);
}

static Int_Chooser g_choosers[] = {
	{
		.index        = LINE_INDEX_SCORE,
		.name         = "Start Score",
		.value        = 0,
		.min_value    = 0,
		.max_value    = GOTCHA_SCORE_MAX-1,
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
		.max_value    = GOTCHA_MODE_MAX-1,
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
		.max_value    = GOTCHA_MODE_MAX-1,
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


void screen_configure_game_gotcha_refresh(struct Screen *screen, struct Match *match)
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

	game_screen_set_header(screen, "Gotcha: Configuration", "", "");
	update_and_draw_chooser(screen, &g_chooser_bundle);
}

void screen_configure_game_gotcha_on_enter(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
	struct Game_Gotcha *game = game_gotcha_get_instance();

	get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_SCORE)->value     = game->target_score;
	get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_IN)->value  = game->check_in;
	get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_OUT)->value = game->check_out;
}

void screen_configure_game_gotcha_on_exit(struct Screen *screen, struct Match *match)
{
	(void) screen;
	(void) match;
	struct Game_Gotcha *game = game_gotcha_get_instance();

	game->target_score = get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_SCORE)->value;
	game->check_in     = get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_IN)->value;
	game->check_out    = get_chooser_from_bundle(&g_chooser_bundle, LINE_INDEX_CHECK_OUT)->value;

	player_list_reset_wins(&match->player_list);
	match->legs_played   = 0;
	g_index_first_player = -1;
}


