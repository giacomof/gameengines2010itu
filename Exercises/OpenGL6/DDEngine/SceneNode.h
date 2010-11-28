#ifdef __DDEngine
#    define sceneNode_D __declspec(dllexport)
#else
#    define sceneNode_D __declspec(dllimport)
#endif

#ifndef sceneNode__H__
#define sceneNode__H__

#include "assetManager.h"
#include "globals.h"
#include "linearAlgebra.h"			
#include "transformation.h"
#include "sceneObject.h"
#include <string>
#include <list>
#include <stdlib.h>
#include <SDL.h>						
#include <SDL_opengl.h>					
#include <SDL_thread.h>
#include <SDL_mutex.h>
#include "glut.h"

#include <btBulletDynamicsCommon.h>

using namespace std;


class sceneNode_D SceneNode 
{
	public:

		SDL_mutex *mutex_node;					
		
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
		void release() {}

		void update(float dt);
		void destroy(void);

		// add a child
		void addChild( SceneNode * pNode );
		// set parent node
		void setParent ( SceneNode * pNode );
		// get parent node
		SceneNode* getParent(void);
		// set the node name
		void setName(char * name);
		// get the node name
		std::string getName(void);

		bool isVisible(void);
		void setVisible(bool b);

		void addSceneObject(SceneObject * g);
		SceneObject * getSceneObject();

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

		void setShadingProgram(char * programName);
		void activateShadingProgram(bool active);

		float * tranM;
		btTransform trans;
		string nodeNameString;
		static unsigned int getNodeCount(void);
		SceneNode * findSceneNodeByName(string name);

	protected:
		bool visible;							// The node should be drawn or not
		int id;									// Unique id
		SceneNode * parentNode;					// Parent Node
		list<SceneNode*> childList;				// List of child Nodes
		Transformation nodeTransformation;		// Transformation of the Node
		SceneObject * geometry;					// Mesh to render

		char * shadingProgramName;
		bool shadingIsActive;

		btRigidBody * physicsGeometry;		
		void updateRigidBody(void);

};


class sceneNode_D Root : public SceneNode
{
	public: 


		static list<SceneNode*> childOfRootList;
		static unsigned int id;
		static string nodeName;

		// Singleton Definitions
		static Root _instance;
		static SDL_mutex * rootMutex;
		Root(void) { &getInstance(); }
		Root(const Root &getInstance());
		Root & operator=(Root &getInstance());
		static Root &getInstance();
		
		// Root methods
		SceneNode * findSceneNodeByName(string name);
		void addChild( SceneNode * pNode );
		static void setParent( SceneNode * pNode );
		static SceneNode * getParent(void);
		static void update(float dt);
		static void drawGeometry(void);

};

#endif