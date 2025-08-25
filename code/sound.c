#include "sound.h"

#include "config.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "libcutils/logger.h"

static Mix_Music *sound_load_file(const char *filename)
{
	char filepath[1024];
	snprintf(filepath, sizeof(filepath), "%s/sounds/%s",  g_config.resources_dir, filename);

	return Mix_LoadMUS(filepath);
}

Result sound_init(void)
{

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		return result_make(false, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	return result_make_success();
}

void sound_play_game_on(void)
{
	// TODO: move soundfile to global array and free when closing
	// application
	Mix_Music *music = sound_load_file("game_on.mp3");

	if (music != NULL) {
		Mix_PlayMusic(music, 0);
	}
}


void sound_play_score(struct Dart_Hit *hit)
{
	// TODO: move soundfile to global array and free when closing
	// application
	Mix_Music *music_type  = NULL;
	Mix_Music *music_value = NULL;

	switch (hit->field_type) {
		//case DARTS_SINGLE: music_type = sound_load_file("single.mp3"); break;
		case DARTS_SINGLE: music_type = NULL; break;
		case DARTS_DOUBLE: music_type = sound_load_file("double.mp3"); break;
		case DARTS_TRIPPLE: music_type = sound_load_file("tripple.mp3"); break;
		default: music_type = NULL; break;
	}

	char value_filename[255];
	snprintf(value_filename, sizeof(value_filename), "%d.mp3", hit->field_value);

	music_value = sound_load_file(value_filename);

	if (hit->field_value == 0) {
		if (music_value != NULL) {
			Mix_PlayMusic(music_value, 0);
		}
	}
	else {
		if (music_type!= NULL) {
			Mix_PlayMusic(music_type, 0);

			while (Mix_PlayingMusic()) {
				SDL_Delay(10); // Delay to avoid busy waiting
			}
		}
		if (music_value != NULL) {
			Mix_PlayMusic(music_value, 0);
		}
	}
}
