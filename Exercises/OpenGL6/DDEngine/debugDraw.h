#ifdef __DDEngine
#    define DebugDraw_D __declspec(dllexport)
#else
#    define DebugDraw_D __declspec(dllimport)
#endif

#ifndef DebugDraw__H
#define DebugDraw__H

#include <btBulletCollisionCommon.h>
#include "SDL_opengl.h"
#include <iostream>

#pragma warning(disable: 4251)

class DebugDraw_D DebugDraw : public btIDebugDraw 
{
public:
	DebugDraw(void);
	~DebugDraw(void);

	void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3 &color);
	void DebugDraw::setDebugMode(int mode);
	int  DebugDraw::getDebugMode(void) const;
	void DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void DebugDraw::reportErrorWarning(const char* text);
	void DebugDraw::draw3dText(const btVector3& location, const char* text);

private:
	int mode; 
};

#endif