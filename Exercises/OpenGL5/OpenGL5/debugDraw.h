#pragma once

#include <btBulletCollisionCommon.h>

class DebugDraw : public btIDebugDraw 
{
public:
	DebugDraw(void);
	~DebugDraw(void);

	void DebugDraw::drawLine(const btVector3& from, const btVector3& to);
	void DebugDraw::setDebugMode(int mode);
	int DebugDraw::getDebugMode(void) const;

private:
	int mode; 
};

