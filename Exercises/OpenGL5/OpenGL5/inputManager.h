#pragma once

#ifndef inputManager__H__
#define inputManager__H__

#include <SDL.h>
#include "messagePump.h"
#include "controller.h"
#include "assetManager.h"

class inputManager
{
public:
	static inputManager _instance;
	int update(void);

	inputManager() { &getInstance(); }
	~inputManager() { /*SDL_DestroyMutex( inputManager::mutex_event );*/  } 
	inputManager(const inputManager &getInstance());   
	
	inputManager & operator=(inputManager &getInstance());
	static inputManager &getInstance();

	static unsigned int count;
	static SDL_mutex * mutex_event;

private:
	static bool active;

	void keyPress(SDL_Event &currentEvent);
	void mousePress(SDL_Event &currentEvent);
	void mouseMotion(SDL_Event &currentEvent);
};

#endif