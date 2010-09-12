#include <stddef.h>
#include <stdlib.h>

using namespace std;

class SceneNode 
{
	public:
		// constructor
		SceneNode() { nodeCount++; }
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
		// set parent node
		void setParent ( SceneNode* pNode );
		// get parent node
		SceneNode* getParent ();

		
		static int nodeCount;
}