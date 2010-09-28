#include "sceneObject.h"

class entitySpectator : public SceneObject
{
private:
	bool shouldMoveForward;
	bool shouldMoveBackward;
	bool shouldStrafeLeft;
	bool shouldStrafeRight;
	float lookUpDownAmount;
	float lookLeftRightAmount;

public:
	virtual void update();
	virtual void moveForward(bool shouldMove);
	virtual void moveBackward(bool shouldMove);
	virtual void strafeLeft(bool shouldMove);
	virtual void strafeRight(bool shouldMove);
	virtual void lookUpDown(float deltaLook);
	virtual void lookLeftRight(float deltaLook);
};