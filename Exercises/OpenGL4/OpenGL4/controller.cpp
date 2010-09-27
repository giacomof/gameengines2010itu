#include "controller.h"

// Declare static variables
Controller Controller::_instance;
SDL_mutex * Controller::mutex_controller;

Controller &Controller::getInstance()
{
  return _instance;
}