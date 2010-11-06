#define sound_D __declspec(dllimport)

#ifndef sound_H
#define sound_H

#include <SDL.h>
#include <SDL_audio.h>					// Header File for SDL audio library

#define NUM_SOUNDS 2

// Type for Sound data
static struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

int soundInit(void);

int soundExit(void);

void soundPlayFile(char *file);

void soundMix(void *unused, Uint8 *stream, int len);

#endif