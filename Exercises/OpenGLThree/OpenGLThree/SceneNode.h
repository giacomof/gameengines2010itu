#include <string>
#include <list>

#include "transformation.h"


using namespace std;


class SceneNode 
{
	public:
		// constructor
		SceneNode(void){};
		SceneNode( SceneNode* pNode, string uff );
		// destructor
		virtual ~SceneNode() { destroy(); }
		// delete object
		void release() { delete this; }

		void update(void);
		void destroy(void);

		// add a child
		void addChild( SceneNode pNode );
		// detach a child
		void detachChild( SceneNode* cNode );
		// set parent node
		void setParent ( SceneNode* pNode );
		// get parent node
		SceneNode* getParent(void);
		// set the node name
		void setName(string name);
		// get the node name
		string getName(void);

		void drawGeometry();


	
	protected:
		int id;
		string nodeName;
		SceneNode * parentNode;
		shared_ptr<Transformation> spObj;
		list<SceneNode> childList;

		
};


class Root: public SceneNode
{

	private:
		static Root* instance_ptr;

	public:
		Root(void);
		~Root(void);

		static Root* get_instance();

		// set parent node
		void setParent ( SceneNode* pNode );
		// get parent node
		SceneNode* getParent(void);

		static void drawGeometry();

};

