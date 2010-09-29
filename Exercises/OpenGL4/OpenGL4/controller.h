#include <SDL.h>
#include "entitySpectator.h"

class Controller								// Singleton
{
public:
	static Controller _instance;
	static SDL_mutex *mutex_controller;
	static entitySpectator *playerObject;
 
	Controller() { mutex_controller = SDL_CreateMutex(); playerObject = NULL; &getInstance(); }
	~Controller() { SDL_DestroyMutex( mutex_controller ); } 
	Controller(const Controller &getInstance());
	
	Controller & operator=(Controller &getInstance());
	static Controller &getInstance();
	static void setPlayerObject(entitySpectator *player);
	static void moveForward(bool shouldMove);
	static void moveBackward(bool shouldMove);
	static void strafeLeft(bool shouldMove);
	static void strafeRight(bool shouldMove);
	static void lookDeltaX(float deltaLook);
	static void lookDeltaY(float deltaLook);
	static void lock() { SDL_mutexP( mutex_controller ); }
	static void unlock() { SDL_mutexV( mutex_controller ); }
};