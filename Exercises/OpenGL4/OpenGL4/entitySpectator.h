#include "sceneObject.h"

class entitySpectator : public SceneObject
{
private:
	bool moveForward;
	bool moveBackward;
	bool strafeLeft;
	bool strafeRight;
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