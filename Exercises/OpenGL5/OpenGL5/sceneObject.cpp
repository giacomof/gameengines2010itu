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
	mutex_object = SDL_CreateMutex();
}


SceneObject::~SceneObject(void)
{
	SDL_DestroyMutex( mutex_object );
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
// ******** COLLADA INTERFACE ********** //
// ************************************* //

ColladaInterface::ColladaInterface(ColladaFile * c, unsigned int texture)
{
	mesh = c;
	colladaTexture = texture;
}

ColladaInterface::~ColladaInterface(void)
{
}

void ColladaInterface::drawGeometry(void) {
		
	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, colladaTexture);
	mesh->render();
	glDisable(GL_TEXTURE_2D);

}

void  ColladaInterface::update(void) {
	// update the animation
	// if any
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

// ************************************* //
// ******** LIGHT ********************** //
// ************************************* //

unsigned short Light::lightCount = 0;

Light::Light(void)
{
	lightReference = GL_LIGHT0 + Light::lightCount;

	for(unsigned short i = 0; i < 3; i++)
	{
		ambientColor[i] = 0;
		diffuseColor[i] = 1;
		specularColor[i] = 1;
		direction[i] = 0;
	}
	direction[4] = 1.0f;
	isEnabled = true;

	Light::lightCount++;
}

Light::Light(	bool enabled, bool directional,
				float ambientR, float ambientG, float ambientB,
				float diffuseR, float diffuseG, float diffuseB,
				float specularR, float specularG, float specularB)
{
	lightReference = GL_LIGHT0 + Light::lightCount;

	isEnabled = enabled;
	if (directional) 
	{
		direction[4] = 0.0f;
	}
	else
	{
		for(unsigned short i = 0; i < 3; i++)
		{
			direction[i] = 0.0f;
		}
		direction[4] = 1.0f;
	}

	ambientColor[0] = ambientR;
	ambientColor[1] = ambientG;
	ambientColor[2] = ambientB;

	diffuseColor[0] = diffuseR;
	diffuseColor[1] = diffuseG;
	diffuseColor[2] = diffuseB;

	specularColor[0] = specularR;
	specularColor[1] = specularG;
	specularColor[2] = specularB;

	Light::lightCount++;
}

void Light::drawGeometry(void)
{
	// enable light0
	if (isEnabled) 
	{
		glEnable(lightReference);

		glLightfv(lightReference, GL_AMBIENT, ambientColor);
		glLightfv(lightReference, GL_DIFFUSE, diffuseColor);
		glLightfv(lightReference, GL_SPECULAR, specularColor);

		glLightfv(lightReference, GL_POSITION, direction);
	}
	else
	{
		glDisable(lightReference);
	}
}
	
void Light::setDirection(Vector position)
{
	direction[0] = position.getX();
	direction[1] = position.getY();
	direction[2] = position.getZ();
}
