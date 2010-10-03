#include "inputManager.h"

unsigned int inputManager::count=0;
const float mouse_sensitivity = 0.3;

inputManager inputManager::_instance;

MessagePump messageP = MessagePump::getInstance();

SDL_mutex * inputManager::mutex_event;

inputManager &inputManager::getInstance()
{
	if(count==0) inputManager::mutex_event = SDL_CreateMutex();
	else count++;
	return _instance;
}

int inputManager::update(void)
{
SDL_Event currentEvent;

	while ( !messageP.empty() )
	{
		AssetManager::lockMutex( inputManager::mutex_event );
		currentEvent = messageP.receiveMessage();
		AssetManager::unlockMutex( inputManager::mutex_event );

		switch (currentEvent.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyPress(currentEvent);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mousePress(currentEvent);
			break;
		case SDL_MOUSEMOTION:
			mouseMotion(currentEvent);
			break;
		default:
			break;
		}
	}

	return 0;
}

void inputManager::keyPress(SDL_Event currentEvent)
{
	switch (currentEvent.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		if (currentEvent.type == SDL_KEYDOWN)
			// Let's not quit this way at all actually. It causes exceptions atm
			Controller::getInstance().quit = true;
		break;
	case SDLK_w:
		if (currentEvent.type == SDL_KEYDOWN)
			Controller::getInstance().moveForward(true);
		else
			Controller::getInstance().moveForward(false);
		break;
	case SDLK_s:
		if (currentEvent.type == SDL_KEYDOWN)
			Controller::getInstance().moveBackward(true);
		else
			Controller::getInstance().moveBackward(false);
		break;
	case SDLK_a:
		if (currentEvent.type == SDL_KEYDOWN)
			Controller::getInstance().strafeLeft(true);
		else
			Controller::getInstance().strafeLeft(false);
		break;
	case SDLK_d:
		if (currentEvent.type == SDL_KEYDOWN)
			Controller::getInstance().strafeRight(true);
		else
			Controller::getInstance().strafeRight(false);
		break;
	default:
		break;
	}
}

void inputManager::mousePress(SDL_Event currentEvent)
{
	// Do nothing at this time
}

void inputManager::mouseMotion(SDL_Event currentEvent)
{
	float deltaX = currentEvent.motion.xrel * mouse_sensitivity;
	float deltaY = currentEvent.motion.yrel * mouse_sensitivity;
	
	// Only send if the change is significant
	if ( deltaX > 0.01 || deltaX < 0.01 )
		Controller::getInstance().lookDeltaX(deltaX);

	if ( deltaY > 0.01 || deltaY < 0.01 )
		Controller::getInstance().lookDeltaY(deltaY);
}