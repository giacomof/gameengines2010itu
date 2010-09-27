#include <SDL.h>

class Controller								// Singleton
{
public:
	static Controller _instance;
	static SDL_mutex *mutex_controller;
 
	Controller() { &getInstance(); mutex_controller = SDL_CreateMutex(); }
	~Controller() { SDL_DestroyMutex( mutex_controller ); } 
	Controller(const Controller &getInstance());   
	
	Controller & operator=(Controller &getInstance());
	static Controller &getInstance();

	static void lock() { SDL_mutexP( mutex_controller ); }
	static void unlock() { SDL_mutexV( mutex_controller ); }	
};