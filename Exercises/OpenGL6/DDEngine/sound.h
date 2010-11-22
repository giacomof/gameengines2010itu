
#ifndef sound_H
#define sound_H

#include <SDL.h>
#include <SDL_audio.h>					// Header File for SDL audio library
#include "memoryManager.h"
#include "globals.h"

#define NUM_SOUNDS 2

// Type for Sound data
static struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

__declspec(dllexport) int soundInit(void);

__declspec(dllexport) int soundExit(void);

__declspec(dllexport) void soundPlayFile(char *file);

__declspec(dllexport) void soundMix(void *unused, Uint8 *stream, int len);

#endif;