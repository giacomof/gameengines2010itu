#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <vector>
#include "linearAlgebraDLL.h"
#include "md2Loader.h"

using namespace std;
using namespace linearAlgebraDLL;

#ifndef GEOMETRY
#define GEOMETRY

class Geometry
{
	public:
		Geometry(int flag);
		Geometry(md2Loader * meshMD2, char * filename);
		Geometry(md2Loader * meshMD2, unsigned int texture);
		~Geometry(void);

		void render();
		
		void addVertex(Point * p);

		void setShapeFlag(int flag);
		int getShapeFlag(void);

		void setSphere(float r, int sl, int st);
		float getSphereRadius(void);
		int getSphereSlices(void);
		int getSphereStacks(void);

		unsigned int md2Texture;

		md2Loader * mesh;
		vector<Point*> vertexList;				// List of Vertex

		char * textureFile;
		int shapeFlag;
		float radius;
		int slices, stacks;

};

#endif 