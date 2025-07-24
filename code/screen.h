#ifndef SCREEN_H
#define SCREEN_H

#include "darts.h"
#include "game_screen.h"

#include "libcutils/result.h"

#include <SDL.h>
#include <SDL_ttf.h>

#define SCREEN_FPS            30

#define SCREEN_BORDER_WIDTH   10
#define SCREEN_LOGICAL_WIDTH  800
#define SCREEN_LOGICAL_HEIGHT 600

#define SCREEN_FONT_SIZE_XL   54
#define SCREEN_FONT_SIZE_L    36
#define SCREEN_FONT_SIZE_M    24
#define SCREEN_FONT_SIZE_S    18
#define SCREEN_FONT_SIZE_XS   16


#define MAX_OPTION_VALUE_COUNT  5
#define MAX_OPTION_VALUE_LEN    255





enum Screen_Key {
	DKEY_NONE,
	DKEY_ENTER,
	DKEY_MULTIPLY,
	DKEY_DIVIDE,
	DKEY_MINUS,
	DKEY_PLUS,
	DKEY_0,
	DKEY_1,
	DKEY_2,
	DKEY_3,
	DKEY_4,
	DKEY_5,
	DKEY_6,
	DKEY_7,
	DKEY_8,
	DKEY_9
};

struct Screen_Header {
	char line0[255];
	char line1[255];
	char line2[255];
};


struct Screen {
	SDL_Window     *window;
	SDL_Renderer   *renderer;
	TTF_Font       *font;
	uint32_t       ticks;
	struct Screen_Header header;
	char status[255];
	struct Game_Screen_List game_screen_list;
	enum Screen_Key key_pressed;
};


enum Screen_Color {
	SCREEN_COLOR_BLACK,
	SCREEN_COLOR_GREY
};




Result screen_init(struct Screen *screen, int width, int height);
void screen_destroy(struct Screen *screen);
bool screen_rendering_start(struct Screen *screen);
void screen_rendering_stop(struct Screen *screen);

void screen_set_color(struct Screen *screen, enum Screen_Color color);
void screen_draw_header(struct Screen *screen);
void screen_draw_status(struct Screen *screen);
void screen_draw_text(struct Screen *screen, int x, int y, int font_size, const char *fmt, ...);

void screen_draw_option(
	struct Screen *screen,
	int name_width,
	int value_width,
	int y_index,
	bool is_selected,
	const char *name,
	const char *fmt_value, ...);



#endif // SCREEN_H
