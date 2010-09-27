#include "controller.h"

// Declare static variables
Controller Controller::_instance;
SDL_mutex * Controller::mutex_controller;
entitySpectator * Controller::playerObject;



Controller &Controller::getInstance()
{
  return _instance;
}

void Controller::setPlayerObject(entitySpectator *player)
{
	playerObject = player;
}

void Controller::moveForward(bool shouldMove)
{

}

void Controller::moveBackward(bool shouldMove)
{

}

void Controller::strafeLeft(bool shouldMove)
{

}

void Controller::strafeRight(bool shouldMove)
{

}

void Controller::lookUpDown(float deltaLook)
{

}

void Controller::lookLeftRight(float deltaLook)
{

}