#define entityCamera_D __declspec(dllexport)

#ifndef entityCamera__H
#define entityCamera__H

#include "sceneObject.h"
#include "SceneNode.h"

class entityCamera_D entityCamera : public SceneObject
{
public:
	float vPosition[3];
	float vForward[3];
	float vUp[3];
	float pitch;
	float yaw;

	SceneNode * positionNode;
	bool isFollowingNode;
	
	entityCamera(void);
	~entityCamera(void);

	virtual void update(void);
	virtual void setPosition(float x, float y, float z);
	virtual void setForwardVector(float x, float y, float z);
	virtual void setUpVector(float x, float y, float z);
	virtual void setPitchYaw(float newpitch, float newyaw);

	virtual void setSceneNode(SceneNode *newSceneNode);


	

	SDL_mutex * mutex_camera;
};

#endif