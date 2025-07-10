#ifndef UI_SDL_H
#define UI_SDL_H

#include "darts.h"

#include "libcutils/result.h"


Result ui_sdl_init(int width, int height);
void ui_sdl_destroy(void);
bool ui_sdl_refresh(Game *game);

#endif // UI_SDL_H
