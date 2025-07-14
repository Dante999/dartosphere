#ifndef SCREEN_H
#define SCREEN_H

#include "darts.h"

#include "libcutils/result.h"

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	TTF_Font     *font;
} Screen;


Result screen_init(Screen *screen, int width, int height);
void   screen_destroy(Screen *screen);
bool   screen_refresh(Screen *screen, Match *match);

#endif // SCREEN_H
