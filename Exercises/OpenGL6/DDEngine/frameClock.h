#ifdef __DDEngine
#    define frameClock_D __declspec(dllexport)
#else
#    define frameClock_D __declspec(dllimport)
#endif

#ifndef frameClock__H
#define frameClock__H

#include <deque>
#include <SDL.h>
#include <SDL_mutex.h>
#include "globals.h"
#include "mutexManager.h"


using namespace std;

class frameClock_D frameClock
{
public:
	
	SDL_mutex * clockMutex;

	frameClock();
	void frameUpdate();
	Uint32 getFrameDelta();
	int getFPS();
	int frameListSize();

private:
	
	deque<Uint32> frameDeltaList;
	Uint32 currentTime;
	Uint32 lastTime;
	Uint32 frameDelta;
	int fps;
	float deltaSum;
	float averageDelta;
	int pausedTicks;

};

#endif