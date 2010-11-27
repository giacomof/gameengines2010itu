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

	AssetManager::shaderUsesTexture(md2Texture);

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

ColladaInterface::ColladaInterface(ColladaFile * c, unsigned int texture, ColladaSkeleton * s)
{
	skeleton = s;
	mesh = c;
	colladaTexture = texture;
	currentPose = NULL;

	animationProgress = 0.0f;
	animationRate = 1.0f;

	if (skeleton != NULL)
	{
		currentPose = skeleton->buildSkeleton();
	}
}

ColladaInterface::~ColladaInterface(void)
{
}

void ColladaInterface::drawGeometry(void)
{
	AssetManager::shaderUsesTexture(colladaTexture);
	if (skeleton != NULL && mesh != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, colladaTexture);
		mesh->render(currentPose);
		glDisable(GL_TEXTURE_2D);
	}
	else if (mesh != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, colladaTexture);
		mesh->render();
		glDisable(GL_TEXTURE_2D);
	}
	else if (skeleton != NULL)
	{
		skeleton->traceSkeleton(currentPose);
	}
}

void  ColladaInterface::update(void)
{
	if (skeleton != NULL && currentPose != NULL)
	{
		float animationDelta = 0.03f;
		animationProgress = animationProgress + (animationDelta * animationRate);
		if (animationProgress > 1.0f)
			animationProgress = animationProgress - 1.0f;

		//skeleton->updateSkeleton(currentPose, animationProgress);
	}
}

// ************************************* //
// ************** Sphere *************** //
// ************************************* //

Sphere::Sphere(float rad, int sli, int sta, bool w, unsigned int texture) 
{
	radius = rad;
	slices = sli;
	stacks = sta;
	isWireframe = w;
	textureReference = texture;
}
Sphere::~Sphere(void) 
{
}

void Sphere::drawGeometry() 
{
	AssetManager::shaderUsesTexture(textureReference);
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

Plane::Plane(float w, float h, int sideSubdivisions, unsigned int texture) 
{
	width = w;
	height = h;
	subdivisions = sideSubdivisions;

	dividedWidth = width/subdivisions;
	dividedHeight = height/subdivisions;

	halfWidth = width/2;
	halfHeight = height/2;

	textureReference = texture;
}
Plane::~Plane(void) 
{
}

void Plane::drawGeometry() 
{
	AssetManager::shaderUsesTexture(textureReference);
	glDisable(GL_TEXTURE_2D);
	if(subdivisions == 0) 
	{
		glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			glVertex3f( halfWidth, 0.0f, halfHeight);	
			glNormal3f(0,1,0);
			glVertex3f( halfWidth, 0.0f,-halfHeight);
			glNormal3f(0,1,0);
			glVertex3f(-halfWidth, 0.0f,-halfHeight);
			glNormal3f(0,1,0);
			glVertex3f(-halfWidth, 0.0f, halfHeight);			
		glEnd();

	}
	else
	{
		for(int i = 0; i < subdivisions; i++)
		{
			for(int j = 0; j < subdivisions; j++)
			{
				glBegin(GL_QUADS);
					glNormal3f(0,1,0);
					glVertex3f( -halfWidth+dividedWidth*(i+1), 0.0f, halfHeight-(dividedWidth*j));	
					glNormal3f(0,1,0);
					glVertex3f(-halfWidth+dividedWidth*(i+1), 0.0f, halfHeight-(dividedWidth*(j+1)));
					glNormal3f(0,1,0);
					glVertex3f(-halfWidth+dividedWidth*i, 0.0f, halfHeight-(dividedWidth*(j+1)));
					glNormal3f(0,1,0);
					glVertex3f(-halfWidth+dividedWidth*i, 0.0f, halfHeight-(dividedWidth*j));			
				glEnd();
			}
		}
	}

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

Cube::Cube(float s, unsigned int texture) 
{
	side = s;

	textureReference = texture;
}
Cube::~Cube(void) 
{
}

void Cube::drawGeometry() 
{
	float half = side/2;
	AssetManager::shaderUsesTexture(textureReference);
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

Line::Line(Vector lStart, Vector lEnd, unsigned int texture) 
{
	lineStart = lStart;
	lineEnd = lEnd;
	lineVector = lStart - lEnd;

	textureReference = texture;
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
	AssetManager::shaderUsesTexture(textureReference);
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
	direction[1] = 0.0f;
	direction[3] = 1.0f;
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
		direction[3] = 0.0f;
	}
	else
	{
		for(unsigned short i = 0; i < 3; i++)
		{
			direction[i] = 0.0f;
		}
		direction[3] = 1.0f;
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

// ************************************* //
// ************** Teapot *************** //
// ************************************* //

Teapot::Teapot(float size, bool wireframe, unsigned int texture)
{
	dimension = size;
	wire = wireframe;

	textureReference = texture;
}

void Teapot::setDimension(float size)
{
	dimension = size;
}

float Teapot::getSize(void)
{
	return dimension;
}

void Teapot::drawGeometry(void)
{
	AssetManager::shaderUsesTexture(textureReference);
	glDisable(GL_TEXTURE_2D);	
	glDisable(GL_CULL_FACE);
	
	if(wire)
		glutWireTeapot(dimension);
	else
		glutSolidTeapot(dimension);

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}


SkyBox::SkyBox(unsigned int * texture)
{
	skyBoxTextureList = texture;
}

void SkyBox::drawGeometry(void)
{
	glPushMatrix();
 
	//// Second Move the render space to the correct position (Translate)
	//glTranslatef(position.x,position.y,position.z);
 
	float coord = 1000.0f;
	// TOP Side
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D,skyBoxTextureList[0]);
	AssetManager::shaderUsesTexture(skyBoxTextureList[0]);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-coord ,coord,-coord);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-coord,coord,coord);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( coord,coord,coord); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f( coord ,coord,-coord);
	glEnd();
 
	// FRONT side
	glBindTexture(GL_TEXTURE_2D,skyBoxTextureList[1]);
	AssetManager::shaderUsesTexture(skyBoxTextureList[1]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f( coord, -coord,-coord);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( coord,  coord,-coord); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-coord,  coord,-coord);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-coord, -coord,-coord);
	glEnd();

	// BOTTOM side
	glBindTexture(GL_TEXTURE_2D,skyBoxTextureList[2]);
	AssetManager::shaderUsesTexture(skyBoxTextureList[2]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f,0.0f);  glVertex3f(-coord,-coord,-coord);
		glTexCoord2f(0.0f,1.0f);  glVertex3f(-coord,-coord, coord);
		glTexCoord2f(1.0f,1.0f);  glVertex3f( coord,-coord, coord); 
		glTexCoord2f(1.0f,0.0f);  glVertex3f( coord,-coord,-coord);
	glEnd();

	// BACK side
	glBindTexture(GL_TEXTURE_2D,skyBoxTextureList[3]);
	AssetManager::shaderUsesTexture(skyBoxTextureList[3]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f( coord, -coord,coord);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( coord,  coord,coord); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-coord,  coord,coord);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-coord, -coord,coord);
	glEnd();
 
	// RIGHT side
	glBindTexture(GL_TEXTURE_2D,skyBoxTextureList[4]);
	AssetManager::shaderUsesTexture(skyBoxTextureList[4]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(coord, -coord, coord);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(coord,  coord, coord); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(coord,  coord,-coord);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(coord, -coord,-coord);
	glEnd();

	// LEFT side
	glBindTexture(GL_TEXTURE_2D,skyBoxTextureList[5]);
	AssetManager::shaderUsesTexture(skyBoxTextureList[5]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f,1.0f); glVertex3f(-coord, -coord, coord);	
		glTexCoord2f(0.0f,0.0f); glVertex3f(-coord,  coord, coord); 
		glTexCoord2f(1.0f,0.0f); glVertex3f(-coord,  coord,-coord);
		glTexCoord2f(1.0f,1.0f); glVertex3f(-coord, -coord,-coord);		
	glEnd();
 	glEnable(GL_CULL_FACE);

	glPopMatrix();
}

