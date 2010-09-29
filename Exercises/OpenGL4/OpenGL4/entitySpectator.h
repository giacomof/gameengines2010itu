#include "sceneObject.h"

class entitySpectator : public SceneObject
{
private:
	bool shouldMoveForward;
	bool shouldMoveBackward;
	bool shouldStrafeLeft;
	bool shouldStrafeRight;
	float lookDeltaXAmount;
	float lookDeltaYAmount;

public:
	virtual void update();
	virtual void moveForward(bool shouldMove);
	virtual void moveBackward(bool shouldMove);
	virtual void strafeLeft(bool shouldMove);
	virtual void strafeRight(bool shouldMove);
	virtual void lookDeltaX(float deltaLook);
	virtual void lookDeltaY(float deltaLook);
};