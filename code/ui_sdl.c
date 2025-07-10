#include "ui_sdl.h"

#include <unistd.h>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_ttf.h>

#include "libcutils/logger.h"
#include "libcutils/util_makros.h"

static SDL_Window   *g_window   = NULL;
static SDL_Renderer *g_renderer = NULL;
static TTF_Font     *g_font     = NULL;
static SDL_Color     g_font_fg  = {0, 0 , 0, 255};
//static SDL_Color     g_font_bg  = {255, 255, 255, 255};

//#define FONT_PATH "../resources/fonts/share-tech-mono/ShareTechMono-Regular.ttf"
#define FONT_PATH "../resources/fonts/Breaked-51015334/Breaked.ttf"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 600

#define FONT_SIZE_GAME_NAME    24
#define FONT_SIZE_PLAYER_NAME  18
#define FONT_SIZE_PLAYER_STATS 12
#define FONT_SIZE_PLAYER_SCORE 64
#define FONT_SIZE_DART_SCORE   48

#define FONT_SIZE 24

#define BORDER_WIDTH 10
#define PLAYER_BOX_SIZE ((800-(2+MAX_PLAYER_COUNT)*BORDER_WIDTH)/MAX_PLAYER_COUNT)

#define Y_OFFSET_PLAYER 100

#define TURN_BOX_WIDTH   100
#define TURN_BOX_HEIGHT  50

#define X_OFFSET_TURN   100
#define Y_OFFSET_TURN   400

static void check_sdl(int sdl_error)
{
	if (sdl_error != 0) {
		fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}




//static void render_graphx_buffer(SDL_Renderer *renderer, struct graphx_data *gfx_data)
//{
//	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
//
//	for (uint16_t x=0; x < gfx_data->width; ++x) {
//		for (uint16_t y=0; y < gfx_data->height; ++y) {
//
//			enum graphx_color pixel = graphx_get_pixel(gfx_data, x, y);
//
//			if (pixel == GRAPHX_COLOR_BLACK) {
//				SDL_RenderDrawPoint(renderer, x, y);
//			}
//		}
//	}
//}



static Result ui_sdl_load_ttf_font(const char *filepath)
{
	TTF_Init();
	g_font = TTF_OpenFont(filepath, FONT_SIZE);

	if (g_font == NULL) {
		return result_make(
			false,
			"could not initialize font %s: %s\n",
			filepath, TTF_GetError());
	}

	//const int usable_width =
	//	screen->cfg.width - (2 * screen->cfg.border_width);
	//const int max_char_width = usable_width / CHARS_PER_LINE;

	//int text_w;
	//int text_h;

	//TTF_SizeText(screen->m_font, "0", &text_w, &text_h);

	//int ptsize = (max_char_width * 10) / text_w;

	//TTF_SetFontSize(screen->m_font, ptsize);
	//TTF_SizeText(screen->m_font, "0", &screen->m_font_width, &screen->m_font_height);

	//log_info("loaded font with ptsize %d and the dimensions: w=%d h=%d per char\n",
	//         ptsize, screen->m_font_width, screen->m_font_height);

	return result_make_success();
}

static void ui_sdl_draw_text(int x, int y, int font_size, const char *fmt, ...)
{
	if ( fmt == NULL || strlen(fmt) == 0) {
		return;
	}

	char buffer[1024];

	va_list arg_list;
	va_start(arg_list, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg_list);


	TTF_SetFontSize(g_font, font_size);

	SDL_Surface *tmp_surface = TTF_RenderText_Blended(
			g_font, 
			buffer,
			g_font_fg
			);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(
			g_renderer, 
			tmp_surface);

	SDL_Rect text_rect = {
		.x = x,
		.y = y,
		.w = tmp_surface->w,
		.h = tmp_surface->h
	};

	SDL_FreeSurface(tmp_surface);
	SDL_RenderCopy(g_renderer, texture, NULL, &text_rect);
	SDL_DestroyTexture(texture);
}

static void ui_sdl_show_header(Game *game)
{
	(void) game;
	ui_sdl_draw_text(BORDER_WIDTH, BORDER_WIDTH, FONT_SIZE_GAME_NAME, "Game: X01");
}

#define TURN_Y_TEXT_OFFSET 10
static void ui_sdl_show_turn(Game *game)
{
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255); // Green color

	ui_sdl_draw_text(BORDER_WIDTH, Y_OFFSET_TURN-10, FONT_SIZE_DART_SCORE, "Turn: ");
	const int x_start = 100;

			for (size_t i=0; i < ARRAY_SIZE(game->player_turn.dart); ++i) {
		Dart *dart = &game->player_turn.dart[i];

		const int x = x_start + (1+i)*BORDER_WIDTH + i*TURN_BOX_WIDTH;
		const int y = Y_OFFSET_TURN;


		// Define an outlined rectangle
		SDL_Rect outlineRect = {x, y, TURN_BOX_WIDTH, TURN_BOX_HEIGHT}; // x, y, width, height
		if (dart->is_active) {
			SDL_SetRenderDrawColor(g_renderer, 210, 210, 210, 255);
			SDL_RenderFillRect(g_renderer, &outlineRect);
		}
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255); // Green color
		SDL_RenderDrawRect(g_renderer, &outlineRect); // Draw outlined rectangle


		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255); // Green color
		if (dart->score != -1) {
			ui_sdl_draw_text(x+10, y-10, FONT_SIZE_DART_SCORE, "%c%d", 
				multiplicator_as_char(dart->multiplicator), 
				dart->score);
		}
		else if (dart->score == 0) {
			ui_sdl_draw_text(x+10, y, FONT_SIZE_DART_SCORE, "MISS");
		}

	}

}

static void ui_sdl_show_players(Game *game)
{
	for (size_t i=0; i < game->player_count; ++i) {
		Player *player = &game->players[i];

		const int x = (1+i)*BORDER_WIDTH + (i*PLAYER_BOX_SIZE);
		const int y = Y_OFFSET_PLAYER;


		// Define an outlined rectangle
		SDL_Rect outlineRect = {x, y, PLAYER_BOX_SIZE, PLAYER_BOX_SIZE}; // x, y, width, height
		if (player->is_active) {
			SDL_SetRenderDrawColor(g_renderer, 210, 210, 210, 255);
			SDL_RenderFillRect(g_renderer, &outlineRect);
		}
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255); // Green color
		SDL_RenderDrawRect(g_renderer, &outlineRect); // Draw outlined rectangle

		
		// Present the renderer
		//SDL_RenderPresent(g_renderer);
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255); // Green color
		ui_sdl_draw_text(x+10, y+10, FONT_SIZE_PLAYER_NAME, player->name);
		ui_sdl_draw_text(x+10, y+30, FONT_SIZE_PLAYER_SCORE, "%d", player->score);
		ui_sdl_draw_text(x+10, y+120, FONT_SIZE_PLAYER_STATS, "wins: %d", player->legs_won);
	}
}

Result ui_sdl_init(int width, int height)
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
	g_window = SDL_CreateWindow("Dartosphere",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN);
	if(!g_window) {
		printf("Window could not be created!\n"
				"SDL_Error: %s\n", SDL_GetError());
	}
	else {
		// Create renderer
		g_renderer  = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if(!g_renderer) {
			printf("Renderer could not be created!\n"
				"SDL_Error: %s\n", SDL_GetError());
		}
		else {
			SDL_RenderSetLogicalSize(g_renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
		}

	}

	Result r = ui_sdl_load_ttf_font(FONT_PATH);
	if (!r.success) {
		//log_error("failed to load font: %s\n", r.msg);
		return r;
	}

	return result_make(true, "");
}

void ui_sdl_destroy(void)
{
	log_info("destroying SDL Window\n");
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
}

bool ui_sdl_refresh(Game *game)
{
	bool quit = false;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			quit = true;
		}
	}

	// Initialize renderer color white for the background
	check_sdl(SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF));

	// Clear screen
	check_sdl(SDL_RenderClear(g_renderer));



	// add drawing routines here
	ui_sdl_show_header(game);
	ui_sdl_show_players(game);
	ui_sdl_show_turn(game);
	// -------------------



	// Update screen
	SDL_RenderPresent(g_renderer);
	SDL_Delay(10);

	return quit;
}
