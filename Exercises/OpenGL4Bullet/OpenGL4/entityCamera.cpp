#include "entityCamera.h"

entityCamera::entityCamera(void)
{
	mutex_object = SDL_CreateMutex();
}

entityCamera::~entityCamera(void)
{
	//SDL_DestroyMutex ( mutex_object );
}

void entityCamera::update()
{
	// Do nothing
}

void entityCamera::setPosition(float x, float y, float z)
{
	vPosition[0] = x;
	vPosition[1] = y;
	vPosition[2] = z;
}

void entityCamera::setForwardVector(float x, float y, float z)
{
	vForward[0] = x;
	vForward[1] = y;
	vForward[2] = z;
}

void entityCamera::setUpVector(float x, float y, float z)
{
	vUp[0] = x;
	vUp[1] = y;
	vUp[2] = z;
}

void entityCamera::setPitchYaw(float newpitch, float newyaw)
{
	pitch = newpitch;
	yaw = newyaw;
}