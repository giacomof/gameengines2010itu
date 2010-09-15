/* Update gamestate */
void update()
{
	//lock variables
	SDL_mutexP ( value_mutex );

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

		printf("\n");
		printf("Forward : [%f, %f, %f]\n",CamForward[0],CamForward[1],CamForward[2]);
		printf("Sideways: [%f, %f, %f]\n",CamSideways[0],CamSideways[1],CamSideways[2]);
		printf("Up      : [%f, %f, %f]\n",CamUp[0],CamUp[1],CamUp[2]);
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

	//release the lock 
	SDL_mutexV ( value_mutex );
}

/* Check key press events */
void keyDown(SDL_keysym *keysym)
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
	default:
		break;
	}
	return;
}

/* Check key release events */ 
void keyUp(SDL_keysym *keysym) 
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
	default:
		break;
	}
}

/* Apply camera matrices */
float* getCamera()
{	
	/*
	float tranM[16];
	Matrix transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),-camPitch).getTranspose();
	transformationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);
	
	transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),-camYaw).getTranspose();
	transformationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);
	
	transformationMatrix = Matrix::generateTranslationMatrix(camPosX, camPosY, camPosZ).getTranspose();
	transformationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);
	*/
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float tranM[16];
	Matrix transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),-camPitch).getTranspose();
	transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),-camYaw).getTranspose() * transformationMatrix;
	transformationMatrix = Matrix::generateTranslationMatrix(camPosX, camPosY, camPosZ).getTranspose() * transformationMatrix;
	transformationMatrix.getMatrix(&tranM[0]);

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}

/* Clamps the camera to a specific pitch and constrains yaw */
void clampCamera()
{
	// Limits camera pitch
	if (camPitch>90.0f)
		camPitch = 90.0f;
	else if (camPitch<-90.0f)
		camPitch = -90.0f;

	// Rolls over yaw
	while(camYaw<=0.0f)
		camYaw += 360.0f;
	while(camYaw>=360.0f)
		camYaw -= 360.0f;
}