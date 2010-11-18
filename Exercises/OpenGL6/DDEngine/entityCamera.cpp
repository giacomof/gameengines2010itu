#include "entityCamera.h"

entityCamera::entityCamera(void)
{
	mutex_camera = SDL_CreateMutex();
	positionNode = new SceneNode();
	isFollowingNode = false;
}

entityCamera::~entityCamera(void)
{
}

void entityCamera::update(void) 
{ 
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
	yaw   = newyaw;
}

void entityCamera::setSceneNode(SceneNode *newSceneNode)
{
	positionNode = newSceneNode;
	isFollowingNode = true;
}
