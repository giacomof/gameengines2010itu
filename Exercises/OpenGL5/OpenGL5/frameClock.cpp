#include "frameClock.h"

void frameClock::frameUpdate()
{
	lastTime = currentTime;
	currentTime = SDL_GetTicks();

	frameDelta = currentTime - lastTime;

	if (lastTime > 200) // If this is the very first frame update, we don't want to save the time yet
	{
		if (frameDeltaList.size() >= 200)
			frameDeltaList.pop_back();

		frameDeltaList.push_front(frameDelta);
	}

	if (frameDeltaList.size() > 0)
	{
		deltaSum = 0.0f;
		
		for (int i = 0; i < frameDeltaList.size(); i++)
		{
			deltaSum += frameDeltaList[i];
		}

		averageDelta = deltaSum / frameDeltaList.size();
		if (averageDelta != 0.0)
			fps = 1000 / averageDelta;
		else
			fps = 0;
	}
}

Uint32 frameClock::getFrameDelta()
{
	if (frameDeltaList.size() > 0)
		return frameDeltaList.front();
	else
		return 0;
}

int frameClock::getFPS()
{
	return fps;
}