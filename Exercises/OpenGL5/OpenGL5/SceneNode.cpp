#include <string>
#include <list>

#include <stdlib.h>
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>
#include <SDL_mutex.h>

#include "glut.h"

#include "sceneNode.h"

static int nodeCount=0;

// Constructor that take a pointer to the parant node, the name of the node
// the initial position of the node
// the initial orientation of the node
SceneNode::SceneNode(	SceneNode * parentNode, string str, SceneObject * g,
						Vector v,
						Vector p_axis, float p_angle)
{
	// variables inizialization
	nodeName = str;
	parentNode = parentNode;

	geometry = g;

	// set parent node
	parentNode->addChild(this);

	// Initialise the member transformation with the position and orientation parameters
	nodeTransformation = Transformation(v,
										p_axis, p_angle);

	visible = true;
	
	// Define the unique identifier of the node
	id = nodeCount;
	nodeCount++;

	mutex_node = SDL_CreateMutex();
	
}

// If the geometry is a MD2 model than update his animation status
// and do so for all his childs
void SceneNode::update(float dt) 
{
	geometry->update();

	list<SceneNode*>::iterator itS;

	for(itS = childList.begin(); itS != childList.end(); itS++) {
			//(*itS)->lock();
			(*itS)->update(dt);
			//(*itS)->unlock();				
	}
	
}

void SceneNode::destroy(void) 
{
		
	list<SceneNode*>::iterator i;
	for(i=childList.begin(); i != childList.end(); ++i) 
	{ 
		//(*i)->lock();
		(*i)->destroy(); 
	}
		
	 this->release();

}

// Add a new child to the node	
void SceneNode::addChild( SceneNode * cNode ) 
{
	cNode->setParent(this);
	childList.push_front(cNode);
}
		
/*	
void SceneNode::detachChild( SceneNode & cNode ) 
{
	// setParent Root
	// delete the SceneNode from the childList
}
*/

// Set a new parent for the node
void SceneNode::setParent( SceneNode * cNode ) 
{
	parentNode = cNode;
	if(parentNode->getName() == "root") {
		Root::childOfRootList.push_front(this);
	}
	
}

// Return the parent of the node
SceneNode* SceneNode::getParent(void)
{
	return parentNode;
}

// Change the name of the node
void SceneNode::setName(string name) 
{
	nodeName = name;
}

// Return the name of the node
string SceneNode::getName(void) 
{
	return nodeName;
}

// Add a geometry to the Scene Node
void SceneNode::addSceneObject(SceneObject * g) 
{
	geometry = g;

}

// Return the Geometry of the Scene Node
SceneObject * SceneNode::getSceneObject()
{
	return geometry;
}


// Apply a rotation about an arbitrary axis to the node
void SceneNode::rotateAboutAxis(Vector p_Axis, float p_Degree)
{
	nodeTransformation.addQuaternionRotation(Quaternion(p_Axis, p_Degree));
}

// Translate the node
void SceneNode::translate(Vector translateVector) 
{	
	nodeTransformation.addTranslation(translateVector);
}

// Scale the node
void SceneNode::scale(float p_sX, float p_sY, float p_sZ)
{
	nodeTransformation.addScaling(p_sX, p_sY, p_sZ);
}

// Apply a shearing to the node
void SceneNode::shear(float p_shXY, float p_shXZ, float p_shYX, float p_shYZ, float p_shZX, float p_shZY)
{
	nodeTransformation.addShearing(p_shXY, p_shXZ, p_shYX, p_shYZ, p_shZX, p_shZY);
}


// Apply the trasformation of the node and then draw it
void SceneNode::drawGeometry()
{
	applyTransformation();
	
	if(isVisible()) {
		geometry->drawGeometry();
	}

	list<SceneNode*>::iterator itS;
	for(itS = childList.begin(); itS != childList.end(); itS++) {
			//(*itS)->lock();
			(*itS)->drawGeometry();
			//(*itS)->unlock();
	}
	glPopMatrix();
	
}

// Apply the transformation of the node
void SceneNode::applyTransformation()
{
	float tranM[16];
	

	nodeTransformation.getTransformation().getMatrix(&tranM[0]);
	glPushMatrix();
	glMultMatrixf(&tranM[0]);

}

// Apply the inverse transformation of the node
void SceneNode::removeTransformation()
{
	float tranM[16];
	

	nodeTransformation.getInverseTransformation().getMatrix(&tranM[0]);
	glPushMatrix();
	glMultMatrixf(&tranM[0]);

}

// Return the visibility status of the Scene Node
bool SceneNode::isVisible(void)
{
	return visible;
}

// Change the visibility status of the Scene Node
void SceneNode::setVisible(bool b) {
	
	visible = b;

}

unsigned int SceneNode::getNodeCount(void) 
{
	return nodeCount;
}

SDL_mutex * Root::rootMutex;

// Static member initialization. 
Root Root::_instance;
list<SceneNode*> Root::childOfRootList;
unsigned int id;



Root &Root::getInstance()
{
	return _instance;
}

// Method for drawing the entire world moving along the scene graph
void Root::drawGeometry()
{
	list<SceneNode*>::iterator itS;

	for(itS = childOfRootList.begin(); itS != childOfRootList.end(); itS++) {
		AssetManager::lockMutex( rootMutex );
			(*itS)->drawGeometry();
		AssetManager::unlockMutex( rootMutex );
	}

	glPopMatrix();
	
}

// Update the transformation status of all the child nodes of root
void Root::update(float dt)
{
	list<SceneNode*>::iterator itS;

	for(itS = childOfRootList.begin(); itS != childOfRootList.end(); itS++) {
			AssetManager::lockMutex( rootMutex );
			(*itS)->update(dt);
			AssetManager::unlockMutex( rootMutex );				
	}
	
}