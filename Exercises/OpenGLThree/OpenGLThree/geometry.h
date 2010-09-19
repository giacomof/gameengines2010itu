#include <list>
#include "linearAlgebraDLL.h"

using namespace std;
using namespace linearAlgebraDLL;

class Geometry
{
	public:
		Geometry(void);
		~Geometry(void);

		void addVertex(Point * p);

	private:
		list<Point*> vertexList;				// List of Vertex
};

