#include "screen.h"

#include <unistd.h>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_ttf.h>

#include "libcutils/logger.h"
#include "libcutils/util_makros.h"

static SDL_Color g_color_black  = {0, 0 , 0, 255};
//static SDL_Color     screen->font_bg  = {255, 255, 255, 255};

//#define FONT_PATH "../resources/fonts/share-tech-mono/ShareTechMono-Regular.ttf"
//#define FONT_PATH "../resources/fonts/Breaked-51015334/Breaked.ttf"
//#define FONT_PATH "../resources/fonts/martius-font/Martius-LV9L4.ttf"
//#define FONT_PATH "../resources/fonts/xolonium-font/XoloniumBold-xKZO.ttf"
#define FONT_PATH "../resources/fonts/pasti-font/PastiOblique-7B0wK.otf"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 600

#define FONT_SIZE_XL   64-10
#define FONT_SIZE_L    48-8
#define FONT_SIZE_M    24-0
#define FONT_SIZE_S    18-0
#define FONT_SIZE_XS   16-0


#define FONT_SIZE_PLAYER_STATS FONT_SIZE_XS
#define FONT_SIZE_PLAYER_NAME  FONT_SIZE_S
#define FONT_SIZE_GAME_MODS    FONT_SIZE_S
#define FONT_SIZE_GAME_STATS   FONT_SIZE_M
#define FONT_SIZE_GAME_NAME    FONT_SIZE_L
#define FONT_SIZE_DART_SCORE   FONT_SIZE_L
#define FONT_SIZE_PLAYER_SCORE FONT_SIZE_XL



#define BORDER_WIDTH 10
#define PLAYER_BOX_SIZE ((800-(2+MAX_PLAYER_COUNT)*BORDER_WIDTH)/MAX_PLAYER_COUNT)

#define Y_OFFSET_HEADER BORDER_WIDTH
#define Y_OFFSET_PLAYER 120
#define Y_OFFSET_TURN   400

#define TURN_BOX_WIDTH   100
#define TURN_BOX_HEIGHT  50


typedef enum {
	SCREEN_COLOR_BLACK,
	SCREEN_COLOR_GREY
} Screen_Color;

static void screen_set_color(Screen *screen, Screen_Color color)
{
	switch(color) {
	case SCREEN_COLOR_BLACK: 
		SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
		break;
	case SCREEN_COLOR_GREY:
		SDL_SetRenderDrawColor(screen->renderer, 210, 210, 210, 255);
		break;
	}
}

static void check_sdl(int sdl_error)
{
	if (sdl_error != 0) {
		fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

static Result screen_load_ttf_font(Screen *screen, const char *filepath)
{
	TTF_Init();
	screen->font = TTF_OpenFont(filepath, 24);

	if (screen->font == NULL) {
		return result_make(
			false,
			"could not initialize font %s: %s\n",
			filepath, TTF_GetError());
	}


	return result_make_success();
}


static void screen_draw_text(Screen *screen, int x, int y, int font_size, const char *fmt, ...)
{
	if ( fmt == NULL || strlen(fmt) == 0) {
		return;
	}

	char buffer[1024];

	va_list arg_list;
	va_start(arg_list, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg_list);


	TTF_SetFontSize(screen->font, font_size);

	SDL_Surface *tmp_surface = TTF_RenderText_Blended(
			screen->font,
			buffer,
			g_color_black
			);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(
			screen->renderer,
			tmp_surface);

	SDL_Rect text_rect = {
		.x = x,
		.y = y,
		.w = tmp_surface->w,
		.h = tmp_surface->h
	};

	SDL_FreeSurface(tmp_surface);
	SDL_RenderCopy(screen->renderer, texture, NULL, &text_rect);
	SDL_DestroyTexture(texture);
}

static void screen_show_header(Screen *screen, Match *match)
{
	const int x = BORDER_WIDTH;
	int       y = Y_OFFSET_HEADER;

		
	screen_draw_text(screen, x, y, FONT_SIZE_GAME_NAME, "Game: %s", match->game->name);
	y+= FONT_SIZE_GAME_NAME+5;

	screen_draw_text(screen, x, y, FONT_SIZE_GAME_MODS, match->game->modifiers);
	y+= FONT_SIZE_GAME_STATS+5;

	screen_draw_text(screen, x, y, FONT_SIZE_GAME_STATS, "Round: %03zu   Legs for win: %zu", match->round, match->legs_for_win);


}

#define X_OFFSET_TURN   120
#define TURN_Y_TEXT_OFFSET 10
static void screen_show_turn(Screen *screen, Match *match)
{
	screen_set_color(screen, SCREEN_COLOR_BLACK);
	screen_draw_text(screen, BORDER_WIDTH, Y_OFFSET_TURN, FONT_SIZE_DART_SCORE, "Turn: ");

	const int x_start = BORDER_WIDTH + 130;

	for (size_t i=0; i < ARRAY_SIZE(match->player_turn.dart); ++i) {
		Dart *dart = &match->player_turn.dart[i];

		const int x = x_start + (1+i)*BORDER_WIDTH + i*TURN_BOX_WIDTH;
		const int y = Y_OFFSET_TURN;


		SDL_Rect outlineRect = {x-12, y-10, TURN_BOX_WIDTH, TURN_BOX_HEIGHT}; // x, y, width, height

		if (dart->is_active) {
			screen_set_color(screen, SCREEN_COLOR_GREY);
			SDL_RenderFillRect(screen->renderer, &outlineRect);
		}

		screen_set_color(screen, SCREEN_COLOR_BLACK);
		SDL_RenderDrawRect(screen->renderer, &outlineRect);

		if (dart->score != -1) {
			screen_draw_text(screen, x, y, FONT_SIZE_DART_SCORE, "%c%d", 
				multiplicator_as_char(dart->multiplicator), 
				dart->score);
		}
		else if (dart->score == 0) {
			screen_draw_text(screen, x, y, FONT_SIZE_DART_SCORE, "MISS");
		}

	}

}

static void screen_show_players(Screen *screen, Match *match)
{
	for (size_t i=0; i < match->player_count; ++i) {
		Player *player = &match->players[i];

		const int x = (1+i)*BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int y = Y_OFFSET_PLAYER;


		// Define an outlined rectangle
		SDL_Rect outlineRect = {x, y, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE}; // x, y, width, height
		if (player->is_active) {
			SDL_SetRenderDrawColor(screen->renderer, 210, 210, 210, 255);
			SDL_RenderFillRect(screen->renderer, &outlineRect);
		}
		SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255); // Green color
		SDL_RenderDrawRect(screen->renderer, &outlineRect); // Draw outlined rectangle

		
		// Present the renderer
		//SDL_RenderPresent(screen->renderer);
		SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255); // Green color
		screen_draw_text(screen, x+10, y+10, FONT_SIZE_PLAYER_NAME, player->name);
		screen_draw_text(screen, x+10, y+50, FONT_SIZE_PLAYER_SCORE, "%d", player->score);
		screen_draw_text(screen, x+10, y+120, FONT_SIZE_PLAYER_STATS, "legs: %d", player->legs_won);
	}
}

Result screen_init(Screen *screen, int width, int height)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return result_make(false, "SDL could not be initialized!\n"
			"SDL_Error: %s\n", SDL_GetError());
	}

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
	// Disable compositor bypass
	if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
	{
		return result_make(false, "SDL can not disable compositor bypass!");
	}
#endif

	// Create window
	screen->window = SDL_CreateWindow("Dartosphere",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN);
	if(!screen->window) {
		printf("Window could not be created!\n"
				"SDL_Error: %s\n", SDL_GetError());
	}
	else {
		// Create renderer
		screen->renderer  = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);
		if(!screen->renderer) {
			printf("Renderer could not be created!\n"
				"SDL_Error: %s\n", SDL_GetError());
		}
		else {
			SDL_RenderSetLogicalSize(screen->renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
		}

	}

	Result r = screen_load_ttf_font(screen, FONT_PATH);
	if (!r.success) {
		//log_error("failed to load font: %s\n", r.msg);
		return r;
	}

	return result_make(true, "");
}

void screen_destroy(Screen *screen)
{
	log_info("destroying SDL Window\n");
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
	SDL_Quit();
}

bool screen_refresh(Screen *screen, Match *match)
{
	bool quit = false;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			quit = true;
		}
	}

	// Initialize renderer color white for the background
	check_sdl(SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0xFF, 0xFF, 0xFF));

	// Clear screen
	check_sdl(SDL_RenderClear(screen->renderer));



	// add drawing routines here
	screen_show_header(screen, match);
	screen_show_players(screen, match);
	screen_show_turn(screen, match);
	// -------------------



	// Update screen
	SDL_RenderPresent(screen->renderer);
	SDL_Delay(10);

	return quit;
}
