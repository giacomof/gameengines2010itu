#pragma once

#ifndef frameClock__H__
#define frameClock__H__

#include <deque>
#include <SDL.h>

using namespace std;

class frameClock
{
public:
	frameClock() { float fps = 0.0f; currentTime = 0; };
	void frameUpdate();
	Uint32 getFrameDelta();
	int getFPS();

private:
	deque<Uint32> frameDeltaList;
	Uint32 currentTime;
	Uint32 lastTime;
	Uint32 frameDelta;
	int fps;
	float deltaSum;
	float averageDelta;
};

#endif