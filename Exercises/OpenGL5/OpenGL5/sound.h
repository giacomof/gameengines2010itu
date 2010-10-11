#pragma once

#ifndef sound_H_
#define sound_H_

#include <SDL.h>
#include <SDL_thread.h>					// Header File for SDL thread library
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