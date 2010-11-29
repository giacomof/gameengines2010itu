#include "frameClock.h"

frameClock::frameClock()
{ 
	float fps = 0.0f; 
	currentTime = 0; 
	clockMutex = SDL_CreateMutex();
};


void frameClock::frameUpdate()
{

	MutexManager::lockMutex(clockMutex);

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

	MutexManager::unlockMutex(clockMutex);
			

}

Uint32 frameClock::getFrameDelta()
{
	if(!Globals::isStopped)
	{

		if (frameDeltaList.size() > 0)
			return frameDeltaList.front();
		else
			return 0;
	}
	else 
		return 0;
}

int frameClock::getFPS()
{
	return fps;
}
