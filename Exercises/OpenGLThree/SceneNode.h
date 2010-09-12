#include <stddef.h>
#include <stdlib.h>

using namespace std;

class SceneNode 
{
	public:
		// constructor
		SceneNode() {}
		// destructor
		virtual ~SceneNode() { destroy(); }
		// delete object
		void release() { delete this; }

		virtual void update();
		virtual void destroy();

		// add a child
		void addChild( SceneNode* pNode );
		// detach a child
		void detachChild( SceneNode* cNode );
}