#include "inputManager.h"

const float mouse_sensitivity = 0.5;

int inputManager::update()
{
SDL_Event currentEvent;

	while ( !MessagePump::getInstance().empty() )
	{
		MessagePump::getInstance().lock();
		currentEvent = MessagePump::getInstance().receiveMessage();
		MessagePump::getInstance().unlock();

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
			//exit(0);
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

/* Update gamestate */
/*void update()
{
	// The initial orientation, to be modified by pitch and yaw
	Vector CamForward	(0.0, 0.0, -1.0	);
	Vector CamSideways	(1.0, 0.0, 0.0	);
	Vector CamUp		(0.0, 1.0, 0.0	);

	// If we need to move, find the actual forward and sideway vectors
	if ( (wKeyPressed+sKeyPressed+aKeyPressed+dKeyPressed) > 0 ) {
		Matrix CamMatrix;
		CamMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),-camPitch).getTranspose();
		CamMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),-camYaw).getTranspose() * CamMatrix;
		
		CamForward = CamMatrix * CamForward;
		CamSideways = CamMatrix * CamSideways;
		CamUp = CamMatrix * CamUp;
		
	}

	// Forward
	if (wKeyPressed && !sKeyPressed) {
		camPosX -= CamForward[0] * camSpeed;
		camPosY -= CamForward[1] * camSpeed;
		camPosZ -= CamForward[2] * camSpeed;
	}
	
	// Backwards
	if (sKeyPressed && !wKeyPressed) {
		camPosX += CamForward[0] * camSpeed;
		camPosY += CamForward[1] * camSpeed;
		camPosZ += CamForward[2] * camSpeed;
	}
	
	// Left
	if (aKeyPressed && !dKeyPressed) {
		camPosX += CamSideways[0] * camSpeed;
		camPosY += CamSideways[1] * camSpeed;
		camPosZ += CamSideways[2] * camSpeed;
	}
	
	// Right
	if (dKeyPressed && !aKeyPressed) {
		camPosX -= CamSideways[0] * camSpeed;
		camPosY -= CamSideways[1] * camSpeed;
		camPosZ -= CamSideways[2] * camSpeed;
	}
	if (lKeyPressed) {
		PlaySound("include/evil_laugh.wav", NULL, SND_ALIAS | SND_APPLICATION);
	}
}*/

/* Check key press events */
/*void keyDown(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
	case SDLK_ESCAPE:
		exit(0);
		break;
	case SDLK_F1:
		SDL_WM_ToggleFullScreen(surface);
		break;
	case SDLK_w:
		wKeyPressed = 1;
		break;
	case SDLK_s:
		sKeyPressed = 1;
		break;
	case SDLK_a:
		aKeyPressed = 1;
		break;
	case SDLK_d:
		dKeyPressed = 1;
		break;
	case SDLK_l:
		lKeyPressed = 1;
		break;
	case SDLK_0:
		md2Demon.SetAnim(0);
		break;
	case SDLK_1:
		md2Demon.SetAnim(1);
		break;
	case SDLK_2:
		md2Demon.SetAnim(2);
		break;
	case SDLK_3:
		md2Demon.SetAnim(3);
		break;
	case SDLK_4:
		md2Demon.SetAnim(4);
		break;
	case SDLK_5:
		md2Demon.SetAnim(5);
		break;
	case SDLK_6:
		md2Demon.SetAnim(6);
		break;
	case SDLK_7:
		md2Demon.SetAnim(7);
		break;
	case SDLK_8:
		md2Demon.SetAnim(8);
		break;
	case SDLK_9:
		md2Demon.SetAnim(9);
		break;
	default:
		break;
	}
	return;
}*/

/* Check key release events */ 
/*void keyUp(SDL_keysym *keysym) 
{
	switch (keysym->sym)
    {
		case SDLK_w:
			wKeyPressed = 0;
			break;
		case SDLK_s:
			sKeyPressed = 0;
			break;
		case SDLK_a:
			aKeyPressed = 0;
			break;
		case SDLK_d:
			dKeyPressed = 0;
			break;
		case SDLK_l:
			lKeyPressed = 0;
			break;
		default:
			break;
	}
}*/