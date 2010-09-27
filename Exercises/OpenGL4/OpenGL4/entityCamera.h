#include "sceneObject.h"

class entityCamera : public SceneObject
{


public:
	virtual void update();
	virtual void setPosition(float x, float y, float z);
	virtual void setForwardVector(float x, float y, float z);
	virtual void setUpVector(float x, float y, float z);
};