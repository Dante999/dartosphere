#include "screen.h"

#include <unistd.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_ttf.h>
#include <SDL_keycode.h>
#include <SDL_timer.h>

#include "libcutils/logger.h"
#include "libcutils/util_makros.h"

static SDL_Color g_color_black  = {0, 0 , 0, 255};
//static SDL_Color     screen->font_bg  = {255, 255, 255, 255};

//#define FONT_PATH "../resources/fonts/share-tech-mono/ShareTechMono-Regular.ttf"
//#define FONT_PATH "../resources/fonts/Breaked-51015334/Breaked.ttf"
//#define FONT_PATH "../resources/fonts/martius-font/Martius-LV9L4.ttf"
//#define FONT_PATH "../resources/fonts/xolonium-font/XoloniumBold-xKZO.ttf"
#define FONT_PATH "../resources/fonts/pasti-font/PastiOblique-7B0wK.otf"
//#define FONT_PATH "../resources/fonts/poppins/Poppins-ExtraBold.ttf"



#define STATUS_BOX_WIDTH    (SCREEN_LOGICAL_WIDTH-(2*SCREEN_BORDER_WIDTH))
#define STATUS_BOX_HEIGHT   50
#define Y_OFFSET_STATUS_MSG (SCREEN_LOGICAL_HEIGHT-SCREEN_BORDER_WIDTH-STATUS_BOX_HEIGHT)

#define TEXT_BORDER 10

void screen_set_color(struct Screen *screen, enum Screen_Color color)
{
	switch(color) {
	case SCREEN_COLOR_BLACK:
		SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
		break;
	case SCREEN_COLOR_GREY:
		//SDL_SetRenderDrawColor(screen->renderer, 210, 210, 210, 255);
		// green
		//SDL_SetRenderDrawColor(screen->renderer, 90, 230, 60, 255);
		SDL_SetRenderDrawColor(screen->renderer, 180, 250, 160, 255);
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

static Result screen_load_ttf_font(struct Screen *screen, const char *filepath)
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


void screen_draw_text(struct Screen *screen, int x, int y, int font_size, const char *fmt, ...)
{
	if ( fmt == NULL || strlen(fmt) == 0) {
		return;
	}

	char buffer[1024];

	va_list arg_list;
	va_start(arg_list, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg_list);
	va_end(arg_list);

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

void screen_draw_text_boxed(struct Screen *screen, int x, int y, int font_size, int min_width, bool is_selected, const char *fmt, ...)
{
	if ( fmt == NULL || strlen(fmt) == 0) {
		return;
	}

	char buffer[1024];

	va_list arg_list;
	va_start(arg_list, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg_list);
	va_end(arg_list);

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

	const int text_width = text_rect.w+2*TEXT_BORDER;

	SDL_Rect outlineRect = {
		.x = x-TEXT_BORDER, 
		.y = y-TEXT_BORDER, 
		.w = MAX(min_width, text_width), // pick greater one
		.h = text_rect.h+2*TEXT_BORDER
	}; 

	if (is_selected) {
		screen_set_color(screen, SCREEN_COLOR_GREY);
		SDL_RenderFillRect(screen->renderer, &outlineRect);
	}

	screen_set_color(screen, SCREEN_COLOR_BLACK);
	SDL_RenderDrawRect(screen->renderer, &outlineRect);

	SDL_FreeSurface(tmp_surface);
	SDL_RenderCopy(screen->renderer, texture, NULL, &text_rect);
	SDL_DestroyTexture(texture);
}

void screen_draw_status(struct Screen *screen)
{
	const int x = SCREEN_BORDER_WIDTH;
	const int y = Y_OFFSET_STATUS_MSG;

	screen_set_color(screen, SCREEN_COLOR_BLACK);

	SDL_Rect outlineRect = {x, y, STATUS_BOX_WIDTH, STATUS_BOX_HEIGHT}; // x, y, width, height
	SDL_RenderDrawRect(screen->renderer, &outlineRect);
	screen_draw_text(screen, x+5, y+5, SCREEN_FONT_SIZE_L, screen->status);
}

void screen_draw_header(struct Screen *screen)
{
	const int x = SCREEN_BORDER_WIDTH;
	int       y = SCREEN_BORDER_WIDTH;;


	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_L, screen->header.line0);
	y+= SCREEN_FONT_SIZE_L+5;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_S, screen->header.line1);
	y+= SCREEN_FONT_SIZE_S+5;

	screen_draw_text(screen, x, y, SCREEN_FONT_SIZE_M, screen->header.line2);
}


static void screen_handle_keypress(struct Screen *screen, SDL_Keysym *key)
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



#define Y_OFFSET_CHOOSER 120
#define CHOOSER_HEIGHT   50
#define CHOOSER_INT_WIDTH 50
void screen_draw_option(
	struct Screen *screen,
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

	char buffer[1024];

	va_list arg_list;
	va_start(arg_list, fmt_value);
	vsnprintf(buffer, sizeof(buffer), fmt_value, arg_list);
	va_end(arg_list);

	screen_draw_text_boxed(screen, x, y, SCREEN_FONT_SIZE_L, value_width, is_selected, "%s", buffer);

}

Result screen_init(struct Screen *screen, int width, int height)
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
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

	return result_make(true, "");
}

void screen_destroy(struct Screen *screen)
{
	log_info("destroying SDL Window\n");
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
	SDL_Quit();
}

bool screen_rendering_start(struct Screen *screen)
{
	screen->ticks = SDL_GetTicks();


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

	return quit;
}

void screen_rendering_stop(struct Screen *screen)
{
	SDL_RenderPresent(screen->renderer);

	const uint32_t ticks_used = SDL_GetTicks() - screen->ticks;

	int32_t delta = (1000/SCREEN_FPS)-ticks_used;

	if (delta < 0) return;

	SDL_Delay((uint32_t)delta);
}

int screen_key_numeric_value(enum Screen_Key key)
{
	switch(key) {
		case DKEY_0: return 0;
		case DKEY_1: return 1;
		case DKEY_2: return 2;
		case DKEY_3: return 3;
		case DKEY_4: return 4;
		case DKEY_5: return 5;
		case DKEY_6: return 6;
		case DKEY_7: return 7;
		case DKEY_8: return 8;
		case DKEY_9: return 9;
		default    : return -1;
	}
}
