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
	virtual void update();
	virtual void setPosition(float x, float y, float z);
	virtual void setForwardVector(float x, float y, float z);
	virtual void setUpVector(float x, float y, float z);
	virtual void setPitchYaw(float newpitch, float newyaw);
};