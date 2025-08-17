#ifndef CONFIG_H
#define CONFIG_H

struct Color {
	int r;
	int g;
	int b;
	int a;
};

struct Config {
	int screen_font_size_xl;
	int screen_font_size_l;
	int screen_font_size_m;
	int screen_font_size_s;
	int screen_font_size_xs;

	char resources_dir[255];
	char font_file[255];

	struct Color screen_color_font;
	struct Color screen_color_highlight;
};

extern struct Config g_config;

void config_init(void);

#endif//  CONFIG_H
