#include "screen.h"

#include "screen_game_on.h"
#include "screen_welcome.h"
#include "screen_player_selection.h"
#include "screen_game_selection.h"

#include <unistd.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_ttf.h>
#include <SDL_keycode.h>

#include "libcutils/logger.h"
#include "libcutils/util_makros.h"

static SDL_Color g_color_black  = {0, 0 , 0, 255};
//static SDL_Color     screen->font_bg  = {255, 255, 255, 255};

//#define FONT_PATH "../resources/fonts/share-tech-mono/ShareTechMono-Regular.ttf"
//#define FONT_PATH "../resources/fonts/Breaked-51015334/Breaked.ttf"
//#define FONT_PATH "../resources/fonts/martius-font/Martius-LV9L4.ttf"
//#define FONT_PATH "../resources/fonts/xolonium-font/XoloniumBold-xKZO.ttf"
#define FONT_PATH "../resources/fonts/pasti-font/PastiOblique-7B0wK.otf"



#define STATUS_BOX_WIDTH    (SCREEN_LOGICAL_WIDTH-(2*SCREEN_BORDER_WIDTH))
#define STATUS_BOX_HEIGHT   50
#define Y_OFFSET_STATUS_MSG (SCREEN_LOGICAL_HEIGHT-SCREEN_BORDER_WIDTH-STATUS_BOX_HEIGHT)

static Game_Screen g_game_screens[] = {
	{
		.id              = GAME_SCREEN_WELCOME,
		.on_enter        = screen_welcome_on_enter,
		.refresh         = screen_welcome_refresh,
		.on_exit         = screen_welcome_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_PLAYERS,
		.previous_screen = GAME_SCREEN_WELCOME
	},
	{
		.id              = GAME_SCREEN_SELECT_PLAYERS,
		.on_enter        = screen_player_selection_on_enter,
		.refresh         = screen_player_selection_refresh,
		.on_exit         = screen_player_selection_on_exit,
		.next_screen     = GAME_SCREEN_SELECT_GAME,
		.previous_screen = GAME_SCREEN_WELCOME
	},
	{
		.id              = GAME_SCREEN_SELECT_GAME,
		.on_enter        = screen_game_selection_on_enter,
		.refresh         = screen_game_selection_refresh,
		.on_exit         = screen_game_selection_on_exit,
		.next_screen     = GAME_SCREEN_WELCOME,
		.previous_screen = GAME_SCREEN_SELECT_PLAYERS
	}
};


void screen_set_color(Screen *screen, Screen_Color color)
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


void screen_draw_text(Screen *screen, int x, int y, int font_size, const char *fmt, ...)
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

static void screen_draw_status(Screen *screen, Match *match)
{
	(void) match;

	const int x = SCREEN_BORDER_WIDTH;
	const int y = Y_OFFSET_STATUS_MSG;

	screen_set_color(screen, SCREEN_COLOR_BLACK);

	SDL_Rect outlineRect = {x, y, STATUS_BOX_WIDTH, STATUS_BOX_HEIGHT}; // x, y, width, height
	SDL_RenderDrawRect(screen->renderer, &outlineRect);
	screen_draw_text(screen, x+5, y+5, SCREEN_FONT_SIZE_L, screen->header_footer.status_text);
}

static void screen_draw_header(Screen *screen, Match *match)
{
	(void) match;

	const int x = SCREEN_BORDER_WIDTH;
	int       y = SCREEN_BORDER_WIDTH;;


	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, screen->header_footer.header_first);
	y+= SCREEN_FONT_SIZE_L+5;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_S, screen->header_footer.header_second);
	y+= SCREEN_FONT_SIZE_S+5;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_M, screen->header_footer.header_third);
}


static void screen_handle_keypress(Screen *screen, SDL_Keysym *key)
{
	switch(key->sym) {

	case SDLK_KP_ENTER    : screen->key_pressed = DKEY_ENTER;    break;
	case SDLK_KP_PLUS     : screen->key_pressed = DKEY_PLUS;     break;
	case SDLK_KP_MINUS    : screen->key_pressed = DKEY_MINUS;    break;
	case SDLK_KP_DIVIDE   : screen->key_pressed = DKEY_DIVIDE;   break;
	case SDLK_KP_MULTIPLY : screen->key_pressed = DKEY_MULTIPLY; break;
	case SDLK_KP_0        : screen->key_pressed = DKEY_0;        break;
	case SDLK_KP_1        : screen->key_pressed = DKEY_1;        break;
	case SDLK_KP_2        : screen->key_pressed = DKEY_2;        break;
	case SDLK_KP_3        : screen->key_pressed = DKEY_3;        break;
	case SDLK_KP_4        : screen->key_pressed = DKEY_4;        break;
	case SDLK_KP_5        : screen->key_pressed = DKEY_5;        break;
	case SDLK_KP_6        : screen->key_pressed = DKEY_6;        break;
	case SDLK_KP_7        : screen->key_pressed = DKEY_7;        break;
	case SDLK_KP_8        : screen->key_pressed = DKEY_8;        break;
	case SDLK_KP_9        : screen->key_pressed = DKEY_9;        break;

	}
}


static Game_Screen *get_current_screen(Screen *screen)
{
	for (size_t i=0; i < screen->game_screens_count; ++i) {
		Game_Screen *ptr = &screen->game_screens[i];

		if (ptr->id == screen->current_screen) {
			return ptr;
		}
	}

	assert(false && "no current screen found!");
}

void screen_previous(Screen *screen, Match *match)
{
	get_current_screen(screen)->on_exit(screen, match);
	screen->current_screen = get_current_screen(screen)->previous_screen;
	get_current_screen(screen)->on_enter(screen, match);

}

void screen_next(Screen *screen, Match *match)
{
	get_current_screen(screen)->on_exit(screen, match);
	screen->current_screen = get_current_screen(screen)->next_screen;
	get_current_screen(screen)->on_enter(screen, match);
}

static void screen_handle_states(Screen *screen, Match *match)
{
	get_current_screen(screen)->refresh(screen, match);

	screen_draw_header(screen, match);
	screen_draw_status(screen, match);
}


#define Y_OFFSET_CHOOSER 120
#define CHOOSER_HEIGHT   50
#define CHOOSER_INT_WIDTH 50
void screen_draw_option(
	Screen *screen,
	int name_width,
	int value_width,
	int y_index,
	bool is_selected,
	const char *name,
	const char *fmt_value, ...)
{
	int x = SCREEN_BORDER_WIDTH;
	int y = Y_OFFSET_CHOOSER + (y_index * 55);

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "%s", name);
	x += name_width;

	SDL_Rect outlineRect = {x-5, y-5, value_width, CHOOSER_HEIGHT}; // x, y, width, height
	if (is_selected) {
		screen_set_color(screen, SCREEN_COLOR_GREY);;
		SDL_RenderFillRect(screen->renderer, &outlineRect);
	}

	screen_set_color(screen, SCREEN_COLOR_BLACK);;
	SDL_RenderDrawRect(screen->renderer, &outlineRect);

	char buffer[1024];

	va_list arg_list;
	va_start(arg_list, fmt_value);
	vsnprintf(buffer, sizeof(buffer), fmt_value, arg_list);
	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "%s", buffer);

}

void screen_draw_int_chooser(Screen *screen, int x_offset, int y_index, String_Chooser *chooser, bool is_selected)
{
	int x = SCREEN_BORDER_WIDTH;
	int y = Y_OFFSET_CHOOSER + (y_index * 50);

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "%s", chooser->name);
	x += x_offset;

	SDL_Rect outlineRect = {x+5, y+5, CHOOSER_INT_WIDTH, CHOOSER_HEIGHT}; // x, y, width, height
	if (is_selected) {
		screen_set_color(screen, SCREEN_COLOR_GREY);;
		SDL_RenderFillRect(screen->renderer, &outlineRect);
	}

	screen_set_color(screen, SCREEN_COLOR_BLACK);;
	SDL_RenderDrawRect(screen->renderer, &outlineRect);
	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, "%d", chooser->values[chooser->selected]);
}

Result screen_init(Screen *screen, Match *match, int width, int height)
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
		return result_make(false, "Window could not be created!\n"
				"SDL_Error: %s\n", SDL_GetError());
	}

	// Set the window to fullscreen
	//SDL_SetWindowFullscreen(screen->window, SDL_WINDOW_FULLSCREEN_DESKTOP);



	// Create renderer
	screen->renderer  = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);
	if(!screen->renderer) {
		return result_make(false, "Renderer could not be created!\n"
				"SDL_Error: %s\n", SDL_GetError());
	}

	SDL_RenderSetLogicalSize(screen->renderer, SCREEN_LOGICAL_WIDTH, SCREEN_LOGICAL_HEIGHT);


	Result r = screen_load_ttf_font(screen, FONT_PATH);
	if (!r.success) {
		//log_error("failed to load font: %s\n", r.msg);
		return r;
	}

	screen->current_screen     = GAME_SCREEN_WELCOME;
	screen->game_screens       = g_game_screens;
	screen->game_screens_count = ARRAY_SIZE(g_game_screens);
	get_current_screen(screen)->on_enter(screen, match);
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

	screen->key_pressed = DKEY_NONE;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			screen_handle_keypress(screen, &event.key.keysym);
			break;
		}
	}

	// Initialize renderer color white for the background
	check_sdl(SDL_SetRenderDrawColor(screen->renderer, 0xFF, 0xFF, 0xFF, 0xFF));

	// Clear screen
	check_sdl(SDL_RenderClear(screen->renderer));



	// add drawing routines here
	screen_handle_states(screen, match);
	// -------------------



	// Update screen
	SDL_RenderPresent(screen->renderer);
	SDL_Delay(10);

	return quit;
}

void screen_set_header(Screen *screen, const char *first_line, const char *second_line, const char *third_line)
{
	strncpy(screen->header_footer.header_first, first_line, sizeof(screen->header_footer.header_first));
	strncpy(screen->header_footer.header_second, second_line, sizeof(screen->header_footer.header_second));
	strncpy(screen->header_footer.header_third, third_line, sizeof(screen->header_footer.header_third));
}

void screen_set_status(Screen *screen, const char *status)
{
	strncpy(screen->header_footer.status_text, status, sizeof(screen->header_footer.status_text));
}
