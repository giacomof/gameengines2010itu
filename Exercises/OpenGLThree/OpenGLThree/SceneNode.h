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

		virtual void update(void);
		virtual void destroy(void);

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


	
	private:
		int id;
		string nodeName;
		SceneNode * parentNode;
		shared_ptr<Transformation> spObj;
		list<SceneNode> childList;
		
};


class root: public SceneNode
{
public:
	root(void);
	~root(void);

	SceneNode * getChildByName(string name);
	SceneNode * getChildByID(int id);



private:

};

