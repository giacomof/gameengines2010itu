#pragma once

#include <SDL.h>
#include "messagePump.h"
#include "controller.h"

class inputManager
{
public:
	static inputManager _instance;
	int update(void);

	inputManager() { &getInstance(); }
	~inputManager() { SDL_DestroyMutex( mutex_event );  } 
	inputManager(const inputManager &getInstance());   
	
	inputManager & operator=(inputManager &getInstance());
	static inputManager &getInstance();

	static SDL_mutex * mutex_event;

private:
	void keyPress(SDL_Event currentEvent);
	void mousePress(SDL_Event currentEvent);
	void mouseMotion(SDL_Event currentEvent);
};