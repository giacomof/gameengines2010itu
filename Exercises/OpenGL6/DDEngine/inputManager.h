#ifdef __DDEngine
#    define inputManager_D __declspec(dllexport)
#else
#    define inputManager_D __declspec(dllimport)
#endif

#ifndef inputManager__H__
#define inputManager__H__

#include <SDL.h>
#include "messagePump.h"
#include "controller.h"
#include "mutexManager.h"

class inputManager_D inputManager
{
public:

	// External Declarations
	static unsigned int count;
	static SDL_mutex * mutex_event;
	static bool active;

	// Singleton definitions
	static inputManager _instance;
	inputManager() { & getInstance(); }
	inputManager(const inputManager & getInstance());   
	~inputManager() {};
	inputManager & operator=(inputManager & getInstance());
	static inputManager & getInstance();

	int update(void);

private:

	void keyPress(SDL_Event & currentEvent);
	void mousePress(SDL_Event & currentEvent);
	void mouseMotion(SDL_Event & currentEvent);
};

#endif