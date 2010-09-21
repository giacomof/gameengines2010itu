#include <il.h>
#include <ilu.h>
#include <ilut.h>

#include "geometry.h"

using namespace linearAlgebraDLL;


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

// A function to load a bitmap file and return the texture object for that texture
unsigned int Geometry::MakeTexture() {

	/*unsigned int w,h,bpp;
	unsigned char* pixels;

	if(!LoadPcxImage(textureFile,&pixels,&w,&h,&bpp)) {
		std::cout << "ERROR LOADING THE TEXTURE";
		return 0;
	}

	GLenum infmt,outfmt;
	switch(bpp) {
	case 3:
		infmt = GL_RGB;
		outfmt = GL_RGB;
		break;
	case 4:
		infmt = GL_RGBA;
		outfmt = GL_RGBA;
		break;
	case 1:
		infmt = outfmt = GL_ALPHA;
		break;
	case 2:
		infmt = outfmt = GL_RGB5_A1;
		break;
	default:
		free(pixels);
		break;
		return 0;
	}
	*/

	unsigned int tex_obj=0;
	tex_obj = ilutGLLoadImage(textureFile);
	glGenTextures(1,&tex_obj);

	glBindTexture (GL_TEXTURE_2D, tex_obj);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D,0,outfmt,w,h,0,infmt,GL_UNSIGNED_BYTE,pixels);

	return tex_obj;
}