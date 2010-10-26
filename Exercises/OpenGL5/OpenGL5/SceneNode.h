#pragma once

#ifndef sceneNode__H__
#define sceneNode__H__

#include "globals.h"
#include <SDL_mutex.h>
#include <string>
#include <list>
#include "linearAlgebraDLL.h"			// Header File for our math library
#include "transformation.h"
#include "sceneObject.h"
#include "assetManager.h"

#include <btBulletDynamicsCommon.h>

using namespace linearAlgebraDLL;
using namespace std;


class SceneNode 
{
	public:

		SDL_mutex *mutex_node;					// Mutex for the Node
		
		// default constructor
		SceneNode() { mutex_node = SDL_CreateMutex(); };
		// actual constructor
		SceneNode(		SceneNode * parentNode, char * str, SceneObject * g /*= 0*/,
						Vector v,
						Vector p_axis, float p_angle,
						btRigidBody * rigidBody = 0);
		// destructor
		virtual ~SceneNode() { destroy(); }
		// delete object
		void release() { /*SDL_DestroyMutex ( mutex_node );*/ }

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
		void setName(char * name);
		// get the node name
		char * getName(void);

		bool isVisible(void);
		void setVisible(bool b);

		void addSceneObject(SceneObject * g);
		SceneObject* getSceneObject();

		void rotateAboutAxis(Vector p_Axis, float p_Degree);
		void translate(Vector translateVector);
		void scale(float p_sX, float p_sY, float p_sZ);
		void shear(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

		void setPosition(Vector t);
		void setOrientation(Quaternion q);

		Vector getWorldPosition(void);
		Quaternion getWorldOrientation(void);

		Transformation * getTransformation();

		void applyTransformation(void);
		void SceneNode::removeTransformation(void);
		void drawGeometry(void);
		void drawName(void);


		string nodeNameString;
		static unsigned int getNodeCount(void);
		

	protected:
		bool visible;							// The node should be drawn or not
		int id;									// Unique id
		char * nodeName;						// Name
		SceneNode * parentNode;					// Parent Node
		list<SceneNode*> childList;				// List of child Nodes
		Transformation nodeTransformation;		// Transformation of the Node
		SceneObject * geometry;					// Mesh to render

		btRigidBody * physicsGeometry;		
		void updateRigidBody(void);


		
};


class Root : public SceneNode
{
	public: // Singleton
		static Root _instance;
		
		static SDL_mutex * rootMutex;

		Root(void) { &getInstance(); }
		//~Root(void);

		Root(const Root &getInstance());

		Root & operator=(Root &getInstance());
		
		static Root &getInstance();
		// set parent node
		static void setParent( SceneNode * pNode );
		// get parent node
		static SceneNode * getParent(void);

		// Update all the children
		static void update(float dt);
		// Draw the geometry of all the children
		static void drawGeometry(void);

		static list<SceneNode*> childOfRootList;

		static unsigned int id;
		static string nodeName;

};

#endif