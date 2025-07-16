#include "screen_game_on.h"

#include "libcutils/util_makros.h"

#define Y_OFFSET_HEADER     SCREEN_BORDER_WIDTH
#define Y_OFFSET_PLAYER     120
#define Y_OFFSET_TURN       400

#define PLAYER_BOX_SIZE ((800-(2+MAX_PLAYER_COUNT)*SCREEN_BORDER_WIDTH)/MAX_PLAYER_COUNT)

#define TURN_BOX_WIDTH   100
#define TURN_BOX_HEIGHT  50

static void screen_show_header(Screen *screen, Match *match)
{
	const int x = SCREEN_BORDER_WIDTH;
	int       y = Y_OFFSET_HEADER;


	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "Game: %s", match->game_mode->name);
	y+= SCREEN_FONT_SIZE_L+5;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_S, ""/*match->game.modifiers*/);
	y+= SCREEN_FONT_SIZE_S+5;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_M, "Round: %03zu   Legs for win: %zu", match->round, match->legs_for_win);


}


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

static void screen_show_turn(Screen *screen, Match *match)
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
void screen_game_on(Screen *screen, Match *match)
{
	screen_show_header(screen, match);
	screen_show_players(screen, match);
	screen_show_turn(screen, match);

	if (match->key == DKEY_ENTER) match_next_state(match);
}


