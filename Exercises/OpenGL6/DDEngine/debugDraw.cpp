#include "debugDraw.h"


DebugDraw::DebugDraw(void)
{
}


DebugDraw::~DebugDraw(void)
{
}


void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3 &color) 
{ 
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
		glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);	// lighting and color mask
																// need to disable lighting for proper text color
																// not sure about lines
			glColor3f(0.0f, 1.0f, 0.0f); 
			glBegin(GL_LINES);
				glVertex3f(from.getX(), from.getY(), from.getZ());
				glVertex3f(to.getX(), to.getY(), to.getZ());
			glEnd();

		glPopAttrib();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
} 

void DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) 
{ 

	const btVector3 to(PointOnB + normalOnB*distance); 

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();

} 

void DebugDraw::reportErrorWarning(const char* text) 
{ 
	std::cout << text << std::endl;
} 

void DebugDraw::draw3dText(const btVector3& location, const char* text) 
{ 
	// do nothing for now 
} 

void DebugDraw::setDebugMode(int mode) 
{ 
	this->mode = mode; 
} 

int DebugDraw::getDebugMode(void) const 
{ 
	return this->mode; 
} 