#ifndef SCREEN_H
#define SCREEN_H

#include "darts.h"

#include "libcutils/result.h"

#include <SDL.h>
#include <SDL_ttf.h>

#define SCREEN_BORDER_WIDTH   10
#define SCREEN_LOGICAL_WIDTH  800
#define SCREEN_LOGICAL_HEIGHT 600

#define SCREEN_FONT_SIZE_XL   64-10
#define SCREEN_FONT_SIZE_L    48-8
#define SCREEN_FONT_SIZE_M    24-0
#define SCREEN_FONT_SIZE_S    18-0
#define SCREEN_FONT_SIZE_XS   16-0


#define MAX_OPTION_VALUE_COUNT  5
#define MAX_OPTION_VALUE_LEN    255

typedef struct {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	TTF_Font     *font;
} Screen;


typedef enum {
	SCREEN_COLOR_BLACK,
	SCREEN_COLOR_GREY
} Screen_Color;

typedef struct {
	char name[255];
	char values[MAX_OPTION_VALUE_COUNT][MAX_OPTION_VALUE_LEN];
	size_t value_count;
	int selected;
} String_Chooser;

typedef struct {
	char name[255];
	int value;
	int min_value;
	int max_value;
} Int_Chooser;


Result screen_init(Screen *screen, int width, int height);
void   screen_set_color(Screen *screen, Screen_Color color);
void   screen_draw_text(Screen *screen, int x, int y, int font_size, const char *fmt, ...);

void screen_draw_option(
	Screen *screen,
	int name_width,
	int value_width,
	int y_index, 
	bool is_selected,
	const char *name,
	const char *fmt_value, ...);

void   screen_draw_str_chooser(Screen *screen, int y_index, String_Chooser *chooser);
void   screen_draw_int_chooser(Screen *screen, int x_offset, int y_index, String_Chooser *chooser, bool is_selected);


void   screen_destroy(Screen *screen);
bool   screen_refresh(Screen *screen, Match *match);

#endif // SCREEN_H
