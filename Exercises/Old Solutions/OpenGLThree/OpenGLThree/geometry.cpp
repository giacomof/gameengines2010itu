#include "geometry.h"

using namespace linearAlgebraDLL;


Geometry::Geometry(void)
{

}


Geometry::~Geometry(void)
{

}


void Geometry::addVertex(Point * p)
{
	vertexList.push_back(p);
}