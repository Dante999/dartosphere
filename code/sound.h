#ifndef SOUND_H
#define SOUND_H

#include "libcutils/result.h"

#include "player.h"

Result sound_init(void);
void sound_play_game_on(void);
void sound_play_score(struct Dart_Hit *hit);
#endif // SOUND_H
