#ifdef __DDEngine
#    define entityCamera_D __declspec(dllexport)
#else
#    define entityCamera_D __declspec(dllimport)
#endif

#ifndef entityCamera__H__
#define entityCamera__H__

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
	SDL_mutex * mutex_camera;
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

};

#endif