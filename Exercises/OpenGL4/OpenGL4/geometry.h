#include <vector>
#include "linearAlgebraDLL.h"

using namespace std;
using namespace linearAlgebraDLL;

#ifndef GEOMETRY
#define GEOMETRY
class Geometry
{
	public:
		Geometry(void);
		~Geometry(void);

		void addVertex(Point * p);

		vector<Point*> vertexList;				// List of Vertex
};

#endif 