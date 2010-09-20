#include <string>
#include <list>
#include "linearAlgebraDLL.h"			// Header File for our math library
#include "transformation.h"
#include "geometry.h"

using namespace linearAlgebraDLL;
using namespace std;


class SceneNode 
{
	public:
		// default constructor
		SceneNode() {};
		// actual constructor
		SceneNode(		SceneNode * parentNode, string str, Geometry * g,
						float p_tX,		float p_tY,		float p_tZ,
						Vector p_axis, float p_angle);
		// destructor
		virtual ~SceneNode() { }
		// delete object
		void release() { delete this; }

		void update(float dt);
		void destroy(void);

		// add a child
		void addChild( SceneNode * pNode );
		// detach a child
		//void detachChild( SceneNode & cNode );
		// set parent node
		void setParent ( SceneNode * pNode );
		// get parent node
		SceneNode* getParent(void);
		// set the node name
		void setName(string name);
		// get the node name
		string getName(void);

		bool isVisible(void);
		void setVisible(bool b);

		void addGeometry(Geometry * g);
		Geometry* getGeometry();


		//Matrix getTransformation(void); 

		void rotateAboutAxis(Vector p_Axis, float p_Degree);
		void translate(float p_tX, float p_tY, float p_tZ);
		void scale(float p_sX, float p_sY, float p_sZ);
		void shear(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

		void applyTransformation();
		void SceneNode::removeTransformation();
		void drawGeometry();

		

		

	protected:
		bool visible;							// The node should be drawn or not
		int id;									// Unique id
		string nodeName;						// Name
		SceneNode * parentNode;					// Parent Node
		list<SceneNode*> childList;				// List of child Nodes
		Transformation nodeTransformation;		// Transformation of the Node
		Geometry * geometry;					// Mesh to render
		
};


class Root : public SceneNode
{
	public:
		Root(void);
		~Root(void);

		// set parent node
		void setParent ( SceneNode* pNode );
		// get parent node
		SceneNode* getParent(void);
		void update(float dt);
		void drawGeometry();

};

