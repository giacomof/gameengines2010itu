#define entitySpectator_D __declspec(dllexport)

#ifndef entitySpectator__H
#define entitySpectator__H

#include "sceneObject.h"
#include "linearAlgebraDLL.h"
#include "entityCamera.h"
#include "assetManager.h"

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
	
	virtual void update();
	virtual void setCamera(entityCamera *newCamera);

	
	virtual entityCamera* getCamera();
	
	virtual void moveForward(bool shouldMove);
	virtual void moveBackward(bool shouldMove);
	virtual void strafeLeft(bool shouldMove);
	virtual void strafeRight(bool shouldMove);
	virtual void lookDeltaX(float deltaLook);
	virtual void lookDeltaY(float deltaLook);

	
};

#endif