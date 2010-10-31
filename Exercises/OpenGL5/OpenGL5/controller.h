#pragma once

#ifndef controller__H__
#define controller__H__

#include <SDL.h>
#include "entitySpectator.h"
#include "assetManager.h"

class Controller								// Singleton
{
public:
	static Controller _instance;
	static SDL_mutex *mutex_controller;
	static entitySpectator *playerObject;
	static bool quit;
 
	Controller() { & getInstance(); }
	~Controller() {}; 
	Controller(const Controller & getInstance());
	
	Controller & operator=(Controller & getInstance());
	static Controller & getInstance();

	static void setPlayerObject(entitySpectator * player);
	static void moveForward(bool shouldMove);
	static void moveBackward(bool shouldMove);
	static void strafeLeft(bool shouldMove);
	static void strafeRight(bool shouldMove);
	static void lookDeltaX(float deltaLook);
	static void lookDeltaY(float deltaLook);

	static void changeCameraMode(bool following);

	static unsigned int count;
};

#endif