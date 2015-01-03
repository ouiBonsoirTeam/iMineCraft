#ifndef SOUND_H_
#define SOUND_H_

#include "../../../third-party/api/MAC/include/fmod.h"
#include "../../../third-party/api/LINUX/include/fmod.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void playSample(FMOD_SYSTEM *systeme, FMOD_CHANNEL *canal, FMOD_SOUND *son, const char *dir, float volume);
void playMusic(FMOD_SYSTEM *systeme, FMOD_SOUND *son, const char *dir);

#endif
