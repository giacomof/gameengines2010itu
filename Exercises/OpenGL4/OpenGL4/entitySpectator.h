#include "sceneObject.h"

class entitySpectator : public sceneObject
{
private:
	bool moveForward = false;
	bool moveBackward = false;
	bool strafeLeft = false;
	bool strafeRight = false;
	float lookUpDownAmount = 0;
	float lookLeftRightAmount = 0;

public:
	virtual void update();
	virtual void moveForward(bool shouldMove);
	virtual void moveBackward(bool shouldMove);
	virtual void strafeLeft(bool shouldMove);
	virtual void strafeRight(bool shouldMove);
	virtual void lookUpDown(float deltaLook);
	virtual void lookLeftRight(float deltaLook);
};