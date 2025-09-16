#include "sound.h"

#include "config.h"
#include "libcutils/util_makros.h"
#include <SDL.h>
#include <SDL_mixer.h>

#include "libcutils/logger.h"

struct Soundfile {
	Mix_Music *content;
	const char *filename;
};

struct Field_Value_Sound {
	int key;
	struct Soundfile soundfile;
};

struct Field_Type_Sound {
	enum Field_Type key;
	struct Soundfile soundfile;
};

struct Effect_Sound {
	enum Sound_Effect key;
	struct Soundfile soundfile;
};

struct Field_Value_Sound g_field_value_soundfiles[] = {
	{ .key =  0, .soundfile = {NULL, "0.mp3"}},
	{ .key =  1, .soundfile = {NULL, "1.mp3"}},
	{ .key =  2, .soundfile = {NULL, "2.mp3"}},
	{ .key =  3, .soundfile = {NULL, "3.mp3"}},
	{ .key =  4, .soundfile = {NULL, "4.mp3"}},
	{ .key =  5, .soundfile = {NULL, "5.mp3"}},
	{ .key =  6, .soundfile = {NULL, "6.mp3"}},
	{ .key =  7, .soundfile = {NULL, "7.mp3"}},
	{ .key =  8, .soundfile = {NULL, "8.mp3"}},
	{ .key =  9, .soundfile = {NULL, "9.mp3"}},
	{ .key = 10, .soundfile = {NULL, "10.mp3"}},
	{ .key = 11, .soundfile = {NULL, "11.mp3"}},
	{ .key = 12, .soundfile = {NULL, "12.mp3"}},
	{ .key = 13, .soundfile = {NULL, "13.mp3"}},
	{ .key = 14, .soundfile = {NULL, "14.mp3"}},
	{ .key = 15, .soundfile = {NULL, "15.mp3"}},
	{ .key = 16, .soundfile = {NULL, "16.mp3"}},
	{ .key = 17, .soundfile = {NULL, "17.mp3"}},
	{ .key = 18, .soundfile = {NULL, "18.mp3"}},
	{ .key = 19, .soundfile = {NULL, "19.mp3"}},
	{ .key = 20, .soundfile = {NULL, "20.mp3"}},
	{ .key = 25, .soundfile = {NULL, "25.mp3"}},
	{ .key = 50, .soundfile = {NULL, "50.mp3"}}
};

struct Field_Type_Sound g_field_type_soundfiles[] = {
	//{ .field_type =  DARTS_SINGLE , .soundfile = {NULL, "single.mp3"}},
	{ .key = DARTS_DOUBLE , .soundfile = {NULL, "double.mp3"}},
	{ .key = DARTS_TRIPPLE, .soundfile = {NULL, "tripple.mp3"}}
};

struct Effect_Sound g_effect_soundfiles[] = {
	{ .key = SOUND_EFFECT_GAME_ON, .soundfile = {NULL, "game_on.mp3"}},
};

static Mix_Music *sound_load_file(const char *filename)
{
	char filepath[1024];
	snprintf(filepath, sizeof(filepath), "%s/sounds/%s",  g_config.resources_dir, filename);
	log_info("loading soundfile: %s\n", filepath);

	return Mix_LoadMUS(filepath);
}


#define SOUND_GET_FILE(result, array_name, key_value) \
	for (size_t i=0; i < ARRAY_SIZE(array_name); ++i) {     \
                                                                \
		if (array_name[i].key == key_value) {           \
			result = array_name[i].soundfile.content; \
		}                                               \
	}                                                       \


static Mix_Music *sound_get_field_value_soundfile(int score)
{
	Mix_Music *ptr = NULL;
	SOUND_GET_FILE(ptr, g_field_value_soundfiles, score);
	return ptr;
}

static Mix_Music *sound_get_field_type_soundfile(enum Field_Type type)
{
	Mix_Music *ptr = NULL;
	SOUND_GET_FILE(ptr, g_field_type_soundfiles, type);
	return ptr;
}

static Mix_Music *sound_get_effect_soundfile(enum Sound_Effect effect)
{
	Mix_Music *ptr = NULL;
	SOUND_GET_FILE(ptr, g_effect_soundfiles, effect);
	return ptr;
}


#define LOAD_SOUNDFILES(array_name)                                            \
	for (size_t i=0; i < ARRAY_SIZE(array_name); ++i) {      \
		struct Soundfile *sf = &array_name[i].soundfile; \
		sf->content = sound_load_file(sf->filename);                   \
                                                                               \
		if (sf->content == NULL) {                                     \
			return result_make(false,                              \
				"failed to load soundfile: %s", sf->filename); \
		}                                                              \
	}                                                                      \

Result sound_init(void)
{

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		return result_make(false, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	LOAD_SOUNDFILES(g_field_value_soundfiles);
	LOAD_SOUNDFILES(g_field_type_soundfiles);
	LOAD_SOUNDFILES(g_effect_soundfiles);

	return result_make_success();
}

#define FREE_SOUNDFILES(array_name)                                      \
	for (size_t i=0; i < ARRAY_SIZE(array_name); ++i) {              \
		struct Soundfile *sf = &array_name[i].soundfile;         \
		Mix_FreeMusic(sf->content);                              \
		sf->content = NULL;                                      \
	}                                                                \

void sound_destroy(void)
{
	FREE_SOUNDFILES(g_field_value_soundfiles);
	FREE_SOUNDFILES(g_field_type_soundfiles);
	FREE_SOUNDFILES(g_effect_soundfiles);
}

void sound_play_effect(enum Sound_Effect effect)
{
	Mix_Music *music = sound_get_effect_soundfile(effect);

	if (music != NULL) {
		Mix_PlayMusic(music, 0);
	}
}


void sound_play_score(struct Dart_Hit *hit)
{
	Mix_Music *sf_field_type  = sound_get_field_type_soundfile(hit->field_type);
	Mix_Music *sf_field_value = sound_get_field_value_soundfile(hit->field_value);

	if (hit->field_value == 0) {
		if (sf_field_value != NULL) {
			Mix_PlayMusic(sf_field_value, 0);
		}
	}
	else {
		if (sf_field_type != NULL) {
			Mix_PlayMusic(sf_field_type, 0);

			while (Mix_PlayingMusic()) {
				SDL_Delay(10); // Delay to avoid busy waiting
			}
		}
		if (sf_field_value != NULL) {
			Mix_PlayMusic(sf_field_value, 0);
		}
	}
}
