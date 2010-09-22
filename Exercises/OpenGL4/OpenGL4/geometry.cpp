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

Geometry::Geometry(md2Loader * meshMD2, unsigned int texture)
{
	shapeFlag = 2;
	mesh = meshMD2;
	//textureFile = filename;
	md2Texture = texture;
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