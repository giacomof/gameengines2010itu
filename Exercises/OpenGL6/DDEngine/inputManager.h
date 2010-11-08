#ifdef __DDEngine
#    define inputManager_D __declspec(dllexport)
#else
#    define inputManager_D __declspec(dllimport)
#endif

#ifndef inputManager__H
#define inputManager__H

#include <SDL.h>
#include "messagePump.h"
#include "controller.h"
#include "assetManager.h"

class inputManager_D inputManager
{
public:
	static inputManager _instance;
	int update(void);

	inputManager() { & getInstance(); }
	~inputManager() {}; 
	inputManager(const inputManager & getInstance());   
	
	inputManager & operator=(inputManager & getInstance());
	static inputManager & getInstance();

	static unsigned int count;
	static SDL_mutex * mutex_event;

private:
	static bool active;

	void keyPress(SDL_Event & currentEvent);
	void mousePress(SDL_Event & currentEvent);
	void mouseMotion(SDL_Event & currentEvent);
};

#endif