#include <string>
#include <list>

#include "transformation.h"


using namespace std;


class SceneNode 
{
	public:
		// constructor
		SceneNode(){};
		SceneNode( SceneNode * parentNode, string str, 
						float p_tX,		float p_tY,		float p_tZ,
						float p_angleX, float p_angleY, float p_angleZ );
		// destructor
		virtual ~SceneNode() { destroy(); }
		// delete object
		void release() { delete this; }

		void update(void);
		void destroy(void);

		// add a child
		void addChild( SceneNode & pNode );
		// detach a child
		void detachChild( SceneNode & cNode );
		// set parent node
		void setParent ( SceneNode & pNode );
		// get parent node
		SceneNode* getParent(void);
		// set the node name
		void setName(string name);
		// get the node name
		string getName(void);

		void rotate(float p_angleX, float p_angleY, float p_angleZ);
		void translate(float p_tX, float p_tY, float p_tZ);
		void scale(float p_sX, float p_sY, float p_sZ);
		void shear(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

		void applyTransformation();

		void drawGeometry();


	
	protected:
		int id;
		string nodeName;
		SceneNode * parentNode;
		list<SceneNode> childList;
		Transformation nodeTransformation;

		
};


class Root: public SceneNode
{
	public:
		Root(void);
		~Root(void);

		// set parent node
		void setParent ( SceneNode* pNode );
		// get parent node
		SceneNode* getParent(void);

		void drawGeometry();

};

