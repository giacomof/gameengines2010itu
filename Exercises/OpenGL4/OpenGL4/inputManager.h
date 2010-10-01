#pragma once

#include <SDL.h>
#include "messagePump.h"
#include "controller.h"

class inputManager
{
public:
	int update(void);

private:
	void keyPress(SDL_Event currentEvent);
	void mousePress(SDL_Event currentEvent);
	void mouseMotion(SDL_Event currentEvent);
};