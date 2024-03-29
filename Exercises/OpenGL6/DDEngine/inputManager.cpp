#include "inputManager.h"

unsigned int inputManager::count=0;
const float mouse_sensitivity = 0.3;
inputManager inputManager::_instance;
MessagePump messageP = MessagePump::getInstance();
SDL_mutex * inputManager::mutex_event;
bool inputManager::active;

inputManager & inputManager::getInstance()
{
	if(count==0) inputManager::mutex_event = SDL_CreateMutex();
	count++;
	active = true;
	return _instance;
}

int inputManager::update(void)
{
SDL_Event currentEvent;

	while ( !messageP.empty() )
	{
		MutexManager::lockMutex( inputManager::mutex_event );
		currentEvent = messageP.receiveMessage();
		MutexManager::unlockMutex( inputManager::mutex_event );

		if (currentEvent.type == SDL_ACTIVEEVENT)
		{
			if ( currentEvent.active.state & SDL_APPINPUTFOCUS )
			{
				if (currentEvent.active.gain==0)
				{
					active = false;
					SDL_WM_GrabInput(SDL_GRAB_OFF);
					SDL_ShowCursor(SDL_ENABLE);
				}
				else
				{
					active = true;
					SDL_ShowCursor(SDL_DISABLE);
					SDL_WM_GrabInput(SDL_GRAB_ON);
				}
			}
		}

		if (active)
		{
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
				if(!Globals::isStopped) mouseMotion(currentEvent);
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

void inputManager::keyPress(SDL_Event &currentEvent)
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

	case SDLK_c:
		if (currentEvent.type == SDL_KEYDOWN)
			Controller::getInstance().changeCameraMode(false);
		break;

	case SDLK_v:
		if (currentEvent.type == SDL_KEYDOWN)
			Controller::getInstance().changeCameraMode(true);
		break;

	case SDLK_p:
		if (currentEvent.type == SDL_KEYDOWN) {
			if(Globals::isStopped) {
				// recompile the create script
				// and unpause the engine

				scriptHandler = ScriptHandler::getInstance();
				scriptHandler.runScript("scripts/update.js");

				Globals::startEngine();
			} else
				Globals::stopEngine();
		}
			
		break;

	default:
		break;
	}
}

void inputManager::mousePress(SDL_Event &currentEvent)
{
	// Do nothing at this time
}

void inputManager::mouseMotion(SDL_Event &currentEvent)
{
	float deltaX = currentEvent.motion.xrel * mouse_sensitivity;
	float deltaY = currentEvent.motion.yrel * mouse_sensitivity;
	
	// Only send if the change is significant
	if ( deltaX > 0.01 || deltaX < 0.01 )
		Controller::getInstance().lookDeltaX(deltaX);

	if ( deltaY > 0.01 || deltaY < 0.01 )
		Controller::getInstance().lookDeltaY(deltaY);
}