#pragma once

#include <btBulletCollisionCommon.h>

class DebugDraw : public btIDebugDraw 
{
public:
	DebugDraw(void);
	~DebugDraw(void);
};

