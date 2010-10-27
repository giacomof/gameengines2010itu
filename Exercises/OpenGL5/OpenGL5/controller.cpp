#include "controller.h"

// Declare static variables
Controller Controller::_instance;
SDL_mutex * Controller::mutex_controller;
entitySpectator * Controller::playerObject;
unsigned int Controller::count=0;
bool Controller::quit=false;

Controller & Controller::getInstance()
{
	if(count==0) Controller::mutex_controller = SDL_CreateMutex();
	else count++;
	return _instance;
}

void Controller::setPlayerObject(entitySpectator *player)
{
	playerObject = player;
}

void Controller::moveForward(bool shouldMove)
{
	if (playerObject != NULL)
		playerObject->moveForward(shouldMove);
}

void Controller::moveBackward(bool shouldMove)
{
	if (playerObject != NULL)
		playerObject->moveBackward(shouldMove);
}

void Controller::strafeLeft(bool shouldMove)
{
	if (playerObject != NULL)
		playerObject->strafeLeft(shouldMove);
}

void Controller::strafeRight(bool shouldMove)
{
	if (playerObject != NULL)
		playerObject->strafeRight(shouldMove);
}

void Controller::lookDeltaX(float deltaLook)
{
	if (playerObject != NULL)
		playerObject->lookDeltaX(deltaLook);
}

void Controller::lookDeltaY(float deltaLook)
{
	if (playerObject != NULL)
		playerObject->lookDeltaY(deltaLook);
}