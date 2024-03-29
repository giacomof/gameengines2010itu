#ifdef __DDEngine
#    define entitySpectator_D __declspec(dllexport)
#else
#    define entitySpectator_D __declspec(dllimport)
#endif

#ifndef entitySpectator__H
#define entitySpectator__H

#include "assetManager.h"
#include "sceneObject.h"
#include "linearAlgebra.h"
#include "mutexManager.h"
#include "entityCamera.h"


class entitySpectator_D entitySpectator : public SceneObject
{
private:
	entityCamera *camera;

	float position[3];
	float yaw;
	float pitch;
	
	bool shouldMoveForward;
	bool shouldMoveBackward;
	bool shouldStrafeLeft;
	bool shouldStrafeRight;
	
	float lookDeltaXAmount;
	float lookDeltaYAmount;

public:
	entitySpectator(void);
	~entitySpectator(void);
	
	virtual void update(float dt);
	virtual void setCamera(entityCamera *newCamera);

	
	virtual entityCamera * getCamera();
	
	virtual void moveForward(bool shouldMove);
	virtual void moveBackward(bool shouldMove);
	virtual void strafeLeft(bool shouldMove);
	virtual void strafeRight(bool shouldMove);
	virtual void lookDeltaX(float deltaLook);
	virtual void lookDeltaY(float deltaLook);

	
};

#endif