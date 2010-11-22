#include "entitySpectator.h"

entitySpectator::entitySpectator(void)
{
	//mutex_object = SDL_CreateMutex();
	camera = NULL;
	yaw = 0.0f;
	pitch = 0.0f;
	position[0] = 0.0f;
	position[1] = -300.0f;
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
}

void entitySpectator::update()
{
	MutexManager::lockMutex( SceneObject::mutex_object );

	// hardcode speed constant for debug reasons
	float camSpeed = 2;

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
	Vector vForward		(0.0, 0.0, -1.0);
	Vector vSideways	(1.0, 0.0, 0.0);
	Vector vUp			(0.0, 1.0, 0.0);
	
	// Calculate rotation matrix from pitch and yaw
	Matrix oMatrix = Matrix(); 

	// New functions using quaternions
	oMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(1.0,0.0,0.0),pitch)).getTranspose();
	oMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(0.0,1.0,0.0),yaw)).getTranspose() * oMatrix;
	
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
		MutexManager::lockMutex( SceneObject::mutex_object );
			camera->setPosition(position[0],position[1],position[2]);
			camera->setForwardVector(vForward[0],vForward[1],vForward[2]);
			camera->setUpVector(vUp[0],vUp[1],vUp[2]);
			camera->setPitchYaw(pitch, yaw);
		MutexManager::unlockMutex( SceneObject::mutex_object );
	}

	MutexManager::unlockMutex( SceneObject::mutex_object );
}

void entitySpectator::setCamera(entityCamera * newcamera)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		camera = newcamera;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}

entityCamera * entitySpectator::getCamera()
{
	return camera;
}

void entitySpectator::moveForward(bool shouldMove)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		shouldMoveForward = shouldMove;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}

void entitySpectator::moveBackward(bool shouldMove)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		shouldMoveBackward = shouldMove;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}

void entitySpectator::strafeLeft(bool shouldMove)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		shouldStrafeLeft = shouldMove;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}

void entitySpectator::strafeRight(bool shouldMove)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		shouldStrafeRight = shouldMove;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}

void entitySpectator::lookDeltaX(float deltaLook)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		lookDeltaXAmount = lookDeltaXAmount + deltaLook;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}

void entitySpectator::lookDeltaY(float deltaLook)
{
	MutexManager::lockMutex( SceneObject::mutex_object );
		lookDeltaYAmount = lookDeltaYAmount + deltaLook;
	MutexManager::unlockMutex( SceneObject::mutex_object );
}