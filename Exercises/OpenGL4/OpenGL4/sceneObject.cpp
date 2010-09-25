#include "sceneObject.h"

using namespace linearAlgebraDLL;

ILuint texid;
ILboolean success;
GLuint imageT;

// ************************************* //
// ************ SceneObject ************ //
// ************************************* //

SceneObject::SceneObject(void)
{
}


SceneObject::~SceneObject(void)
{
}



// ************************************* //
// ************** MD2 ****************** //
// ************************************* //

md2Interface::md2Interface(md2File * m, unsigned int texture)
{
	mesh = m;
	md2Texture = texture;
}

md2Interface::~md2Interface(void)
{
}

void md2Interface::drawGeometry(void) {
	glEnable(GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, md2Texture);
		mesh->Render();
	glDisable(GL_TEXTURE_2D);	
}

void  md2Interface::update(void) {

	// *************************************************************** //
	// TEMP METHOD, WE HAVE TO CHANGE THIS  WHEN WE MAKE THE FPS CLASS //
	// *************************************************************** //

	//if(dt==NULL) dt = 0.03f;
	mesh->Update(0.03f);
}

// ************************************* //
// ************** Sphere *************** //
// ************************************* //

Sphere::Sphere(float rad, int sli, int sta, bool w) 
{
	radius = rad;
	slices = sli;
	stacks = sta;
	isWireframe = w;
}
Sphere::~Sphere(void) 
{
}

void Sphere::drawGeometry() 
{
	if(isWireframe) {
		glDisable(GL_TEXTURE_2D);	
		glutWireSphere(radius, slices, stacks);
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);	
		glutSolidSphere(radius, slices, stacks);
		glEnable(GL_TEXTURE_2D);
	}
}

float Sphere::getSphereRadius(void) 
{
	return radius;
}
int Sphere::getSphereSlices(void)
{
	return slices;
}

int Sphere::getSphereStacks(void)
{
	return stacks;
}

// ************************************* //
// ************** Plane **************** //
// ************************************* //

Plane::Plane(float w, float h) 
{
	width = w;
	height = h;
}
Plane::~Plane(void) 
{
}

void Plane::drawGeometry() 
{
	glDisable(GL_TEXTURE_2D);	
	glBegin(GL_QUADS);
		glVertex3f( width/2, 0.0f, height/2);			
		glVertex3f(-width/2, 0.0f, height/2);			
		glVertex3f(-width/2, 0.0f,-height/2);			
		glVertex3f( width/2, 0.0f,-height/2);	
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Plane::setDimensions(float w, float h)
{
	width = w;
	height = h;
}

// ************************************* //
// *************** Cube **************** //
// ************************************* //

Cube::Cube(float s) 
{
	side = s;
}
Cube::~Cube(void) 
{
}

void Cube::drawGeometry() 
{
	float half = side/2;

	glDisable(GL_TEXTURE_2D);	
	glBegin(GL_QUADS);
		glVertex3f( half, half,-half );			// Top Right Of The Quad (Top)
		glVertex3f( -half, half,-half );		// Top Left Of The Quad (Top)
		glVertex3f( -half, half, half );		// Bottom Left Of The Quad (Top)
		glVertex3f( half, half, half );			// Bottom Right Of The Quad (Top)

		glVertex3f( half, -half, half );		// Top Right Of The Quad (Bottom)
		glVertex3f( -half, -half, half );		// Top Left Of The Quad (Bottom)
		glVertex3f( -half, -half, -half );		// Bottom Left Of The Quad (Bottom)
		glVertex3f( half, -half, -half );		// Bottom Right Of The Quad (Bottom)

		glVertex3f( half, half, half );			// Top Right Of The Quad (Front)
		glVertex3f( -half, half, half);			// Top Left Of The Quad (Front)
		glVertex3f( -half,-half, half);			// Bottom Left Of The Quad (Front)
		glVertex3f( half,-half, half);			// Bottom Right Of The Quad (Front)

		glVertex3f( half,-half,-half);			// Bottom Left Of The Quad (Back)
		glVertex3f(-half,-half,-half);			// Bottom Right Of The Quad (Back)
		glVertex3f(-half, half,-half);			// Top Right Of The Quad (Back)
		glVertex3f( half, half,-half);			// Top Left Of The Quad (Back)

		glVertex3f(-half, half, half);			// Top Right Of The Quad (Left)
		glVertex3f(-half, half,-half);			// Top Left Of The Quad (Left)
		glVertex3f(-half,-half,-half);			// Bottom Left Of The Quad (Left)
		glVertex3f(-half,-half, half);			// Bottom Right Of The Quad (Left)

		glVertex3f( half, half,-half);			// Top Right Of The Quad (Right)
		glVertex3f( half, half, half);			// Top Left Of The Quad (Right)
		glVertex3f( half,-half, half);			// Bottom Left Of The Quad (Right)
		glVertex3f( half,-half,-half);			// Bottom Right Of The Quad (Right)

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Cube::setSide(float s)
{
	side = s;
}

// ************************************* //
// *************** Line **************** //
// ************************************* //

Line::Line(Vector lStart, Vector lEnd) 
{
	lineStart = lStart;
	lineEnd = lEnd;
	lineVector = lStart - lEnd;
}

void Line::setLine(Vector lStart, Vector lEnd) 
{
	lineStart = lStart;
	lineEnd = lEnd;
	lineVector = lStart - lEnd;
}

Vector Line::getLine(void)
{
	return lineVector;
}

void Line::drawGeometry(void)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
		glVertex3f( lineStart.get(0), lineStart.get(1), lineStart.get(2) );		// origin of the line
		glVertex3f( lineEnd.get(0), lineEnd.get(1), lineEnd.get(2) );			// ending point of the line
	glEnd();
	glEnable(GL_TEXTURE_2D);
}