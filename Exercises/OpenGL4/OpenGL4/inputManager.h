#include <SDL.h>
#include "messagePump.h"

class inputManager
{
public:
	int update();

private:
	void keyPress(SDL_Event currentEvent);
	void mousePress(SDL_Event currentEvent);
	void mouseMotion(SDL_Event currentEvent);
};