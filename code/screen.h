#ifndef SCREEN_H
#define SCREEN_H

#include "darts.h"
#include "game_screen.h"

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

struct Screen {
	SDL_Window     *window;
	SDL_Renderer   *renderer;
	TTF_Font       *font;
	struct {
		char header_first[255];
		char header_second[255];
		char header_third[255];
		char status_text[255];
	} header_footer;
	struct Game_Screen_List game_screen_list;
	enum Screen_Key key_pressed;
};


enum Screen_Color {
	SCREEN_COLOR_BLACK,
	SCREEN_COLOR_GREY
};




Result screen_init(struct Screen *screen, int width, int height);
bool   screen_refresh(struct Screen *screen, struct Match *match);
void   screen_destroy(struct Screen *screen);
void   screen_set_color(struct Screen *screen, enum Screen_Color color);
void   screen_draw_text(struct Screen *screen, int x, int y, int font_size, const char *fmt, ...);

void screen_draw_option(
	struct Screen *screen,
	int name_width,
	int value_width,
	int y_index,
	bool is_selected,
	const char *name,
	const char *fmt_value, ...);



#endif // SCREEN_H
