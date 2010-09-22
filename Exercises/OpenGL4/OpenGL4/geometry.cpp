#include "geometry.h"

using namespace linearAlgebraDLL;

	ILuint texid;
	ILboolean success;
	GLuint imageT;


Geometry::Geometry(int flag)
{
	shapeFlag = flag;
	switch (flag) {

		case 1:
			radius = 1;
			slices = stacks = 10;
			break;

		default:
			break;
	}
}

Geometry::Geometry(md2Loader * meshMD2, char * filename) 
{
	shapeFlag = 2;
	mesh = meshMD2;
	textureFile = filename;
	md2Texture = MakeTexture();
}

void Geometry::render() 
{
	mesh->Render();
}

Geometry::~Geometry(void)
{

}

void Geometry::addVertex(Point * p)
{
	vertexList.push_back(p);
}

void Geometry::setShapeFlag(int flag) 
{
	shapeFlag = flag;
}

int Geometry::getShapeFlag(void)
{
	return shapeFlag;
}


void Geometry::setSphere(float r, int sl, int st)
{
	radius = r;
	slices = sl;
	stacks = st;
}

float Geometry::getSphereRadius(void)
{
	return radius;
}

int Geometry::getSphereSlices(void)
{
	return slices;
}

int Geometry::getSphereStacks(void)
{
	return stacks;
}

// A function to load an image file and return the texture object for it
unsigned int Geometry::MakeTexture() {

	// Initialization of DevIL
	ilInit(); 
	// Generation of one image name 
	ilGenImages(1, &texid); 
	// Binding of image name
	ilBindImage(texid);
	// Loading of image 
	success = ilLoadImage(textureFile);

	if (success)
	{
		// Convert colour (for alpha channel images change to IL_RGBA)
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); 

		if (!success)
		{
			// Error occured 
			//SDL_Quit();
			std::cout << "ERROR" << std::endl;
			return -1;
		}
	}

	// Texture name generation
	glGenTextures(1, &imageT); 
	// Binding of texture name
	glBindTexture(GL_TEXTURE_2D, imageT);
	// Linear interpolation for magnification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Linear interpolation for minifying filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Texture settings
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	return imageT;
}