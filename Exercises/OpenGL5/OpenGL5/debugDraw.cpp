#include "debugDraw.h"
#include "SDL_opengl.h"

DebugDraw::DebugDraw(void)
{
}


DebugDraw::~DebugDraw(void)
{
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to) 
{ 
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(from.getX(), from.getY(), from.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
} 

void DebugDraw::setDebugMode(int mode) 
{ 
	this->mode = mode; 
} 

int DebugDraw::getDebugMode(void) const 
{ 
	return this->mode; 
} 