#ifndef SOUND_H
#define SOUND_H

#include "libcutils/result.h"

#include "player.h"

enum Sound_Effect
{
	SOUND_EFFECT_GAME_ON
};


Result sound_init(void);
void sound_destroy(void);
void sound_play_effect(enum Sound_Effect effect);
void sound_play_score(struct Dart_Hit *hit);

#endif // SOUND_H
