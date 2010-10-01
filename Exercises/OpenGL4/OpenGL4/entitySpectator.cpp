#include "entitySpectator.h"

entitySpectator::entitySpectator(void)
{
	mutex_object = SDL_CreateMutex();
	camera = NULL;
	yaw = 0.0f;
	pitch = 0.0f;
	position[0] = 0.0f;
	position[1] = 0.0f;
	position[2] = 0.0f;
	shouldMoveForward = false;
	shouldMoveBackward = false;
	shouldStrafeLeft = false;
	shouldStrafeRight = false;
	lookDeltaXAmount = 0.0f;
	lookDeltaYAmount = 0.0f;
}

entitySpectator::~entitySpectator(void)
{
	//SDL_DestroyMutex ( mutex_object );
	/*if (camera != NULL)
		delete camera;*/
}
void entitySpectator::update()
{
	this->lock();

	// hardcode speed constant for now
	float camSpeed = 0.5;

	// Update pitch and yaw
	yaw = yaw + lookDeltaXAmount;
	pitch = pitch + lookDeltaYAmount;
	lookDeltaXAmount = 0.0f;
	lookDeltaYAmount = 0.0f;

	// Limits camera pitch
	if (pitch>90.0f)
		pitch = 90.0f;
	else if (pitch<-90.0f)
		pitch = -90.0f;

	// Rolls over yaw
	while(yaw<=0.0f)
		yaw += 360.0f;
	while(yaw>=360.0f)
		yaw -= 360.0f;

	// Starting camera orientation
	Vector vForward		(0.0, 0.0, -1.0	);
	Vector vSideways	(1.0, 0.0, 0.0	);
	Vector vUp			(0.0, 1.0, 0.0	);
	
	// Calculate rotation matrix from pitch and yaw
	Matrix oMatrix;
	oMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),pitch).getTranspose();
	oMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),yaw).getTranspose() * oMatrix;
	
	// Calculate the orientation vectors
	vForward = oMatrix * vForward;
	vSideways = oMatrix * vSideways;
	vUp = oMatrix * vUp;

	// Forward
	if (shouldMoveForward && !shouldMoveBackward) {
		position[0] -= vForward[0] * camSpeed;
		position[1] -= vForward[1] * camSpeed;
		position[2] -= vForward[2] * camSpeed;
	}
	
	// Backwards
	if (shouldMoveBackward && !shouldMoveForward) {
		position[0] += vForward[0] * camSpeed;
		position[1] += vForward[1] * camSpeed;
		position[2] += vForward[2] * camSpeed;
	}
	
	// Left
	if (shouldStrafeLeft && !shouldStrafeRight) {
		position[0] += vSideways[0] * camSpeed;
		position[1] += vSideways[1] * camSpeed;
		position[2] += vSideways[2] * camSpeed;
	}
	
	// Right
	if (shouldStrafeRight && !shouldStrafeLeft) {
		position[0] -= vSideways[0] * camSpeed;
		position[1] -= vSideways[1] * camSpeed;
		position[2] -= vSideways[2] * camSpeed;
	}

	// Update camera
	if (camera != NULL)
	{
		camera->lock();
		camera->setPosition(position[0],position[1],position[2]);
		camera->setForwardVector(vForward[0],vForward[1],vForward[2]);
		camera->setUpVector(vUp[0],vUp[1],vUp[2]);
		camera->setPitchYaw(pitch, yaw);
		camera->unlock();
	}

	this->unlock();
}

void entitySpectator::setCamera(entityCamera *newcamera)
{
	this->lock();
	camera = newcamera;
	this->unlock();
}

entityCamera* entitySpectator::getCamera()
{
	return camera;
}

void entitySpectator::moveForward(bool shouldMove)
{
	this->lock();
	shouldMoveForward = shouldMove;
	this->unlock();
}

void entitySpectator::moveBackward(bool shouldMove)
{
	this->lock();
	shouldMoveBackward = shouldMove;
	this->unlock();
}

void entitySpectator::strafeLeft(bool shouldMove)
{
	this->lock();
	shouldStrafeLeft = shouldMove;
	this->unlock();
}

void entitySpectator::strafeRight(bool shouldMove)
{
	this->lock();
	shouldStrafeRight = shouldMove;
	this->unlock();
}

void entitySpectator::lookDeltaX(float deltaLook)
{
	this->lock();
	lookDeltaXAmount = lookDeltaXAmount + deltaLook;
	this->unlock();
}

void entitySpectator::lookDeltaY(float deltaLook)
{
	this->lock();
	lookDeltaYAmount = lookDeltaYAmount + deltaLook;
	this->unlock();
}