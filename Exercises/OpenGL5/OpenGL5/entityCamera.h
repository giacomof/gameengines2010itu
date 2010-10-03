#pragma once

#include "sceneObject.h"

class entityCamera : public SceneObject
{
public:
	float vPosition[3];
	float vForward[3];
	float vUp[3];
	float pitch;
	float yaw;
	
	entityCamera(void);
	~entityCamera(void);

	virtual void update(void);
	virtual void setPosition(float x, float y, float z);
	virtual void setForwardVector(float x, float y, float z);
	virtual void setUpVector(float x, float y, float z);
	virtual void setPitchYaw(float newpitch, float newyaw);

	SDL_mutex * mutex_camera;
};