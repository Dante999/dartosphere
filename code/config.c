#include "config.h"

#include <string.h>

struct Config g_config = {
	.screen_font_size_xl =  54,
	.screen_font_size_l  =  36,
	.screen_font_size_m  =  24,
	.screen_font_size_s  =  18,
	.screen_font_size_xs =  16,
	.resources_dir =  "../resources",
	.font_file     =  "fonts/pasti-font/PastiOblique-7B0wK.otf",
	.screen_color_font      = {.r=0  , .g=0  , .b=0  , .a=255}, // black
	.screen_color_highlight = {.r=180, .g=250, .b=160, .a=255}  // light green
};

void config_init(void)
{
	// TODO: load from config file
}
