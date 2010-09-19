#include <vector>
#include "linearAlgebraDLL.h"

using namespace std;
using namespace linearAlgebraDLL;

#ifndef GEOMETRY
#define GEOMETRY

class Geometry
{
	public:
		Geometry(int flag);
		~Geometry(void);

		void addVertex(Point * p);

		void setShapeFlag(int flag);
		int getShapeFlag(void);

		void setSphere(float r, int sl, int st);
		float getSphereRadius(void);
		int getSphereSlices(void);
		int getSphereStacks(void);


		vector<Point*> vertexList;				// List of Vertex

	private:
		int shapeFlag;

		float radius;
		int slices, stacks;

};

#endif 