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
SceneNode::SceneNode(	SceneNode * parentNode, char * str, SceneObject * g,
						Vector v,
						Vector p_axis, float p_angle,
						btRigidBody * rigidBody )
{
	// variables inizialization
	nodeName = str;
	nodeNameString = str;

	parentNode = parentNode;

	geometry = g;

	// set parent node
	parentNode->addChild(this);

	// Initialise the member transformation with the position and orientation parameters
	nodeTransformation = Transformation(v,
										p_axis, p_angle);

	physicsGeometry = rigidBody;

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
	//drawName();

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
void SceneNode::setName(char * name) 
{
	nodeName = name;
}

// Return the name of the node
char * SceneNode::getName(void) 
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
	/* OLD CODE
	btTransform trans;
	physicsGeometry->getMotionState()->getWorldTransform(trans);
	btQuaternion actualRotation = trans.getRotation();
	btQuaternion newRotation = btQuaternion(btVector3(p_Axis.get(0), p_Axis.get(1), p_Axis.get(2)), p_Degree);
	btQuaternion final = newRotation * actualRotation;
	physicsGeometry->getWorldTransform().setRotation(final);
	*/

	if (physicsGeometry != 0) {
		//btTransform trans;
		//physicsGeometry->getMotionState()->getWorldTransform(trans);

		//btQuaternion final = btQuaternion(btVector3(p_Axis.get(0), p_Axis.get(1), p_Axis.get(2)), p_Degree) * trans.getRotation();
		//physicsGeometry->getWorldTransform().setRotation(final);

		//btVector3 test = btVector3(this->getWorldPosition().get(0), this->getWorldPosition().get(1), this->getWorldPosition().get(2));

		btTransform trans;
		physicsGeometry->getMotionState()->getWorldTransform(trans);

		btVector3 btActualPosition = physicsGeometry->getWorldTransform().getOrigin();
		Vector actualPosition = Vector(btActualPosition.getX(), btActualPosition.getY(), btActualPosition.getZ());

		Vector newPosition = Matrix::generateAxesRotationMatrix(p_Axis, p_Degree) * actualPosition;

		btVector3 btNewPosition = btVector3(newPosition.get(0), newPosition.get(1), newPosition.get(2));

		physicsGeometry->getWorldTransform().setOrigin(btNewPosition);

		btQuaternion actualRotation = trans.getRotation();
		btQuaternion newRotation = btQuaternion(btVector3(p_Axis.get(0), p_Axis.get(1), p_Axis.get(2)), p_Degree);

		btQuaternion final = newRotation * actualRotation;
		physicsGeometry->getWorldTransform().setRotation(final);

		//newPosition = Vector(physicsGeometry->getWorldTransform().getOrigin().getX(), physicsGeometry->getWorldTransform().getOrigin().getY(), physicsGeometry->getWorldTransform().getOrigin().getZ());
		//cout << newPosition << endl;

	}

	list<SceneNode*>::iterator itS;
	for(itS = childList.begin(); itS != childList.end(); itS++) {
			//(*itS)->lock();
			(*itS)->rotateAboutAxis(p_Axis, p_Degree);
			//(*itS)->unlock();
	}



	nodeTransformation.addQuaternionRotation(Quaternion(p_Axis, p_Degree));
}

// Translate the node
void SceneNode::translate(Vector translateVector) 
{	
	/* OLD CODE
	btTransform trans;
	physicsGeometry->getMotionState()->getWorldTransform(trans);
	btVector3 originalPosition = trans.getOrigin();
	btVector3 newPosition = btVector3(translateVector.get(0), translateVector.get(1), translateVector.get(1));
	btVector3 final = originalPosition + newPosition;
	physicsGeometry->getWorldTransform().setOrigin(final);
	*/

	if (physicsGeometry != 0) {
		btTransform trans;
		physicsGeometry->getMotionState()->getWorldTransform(trans);
		btVector3 final = trans.getOrigin() + btVector3(translateVector.get(0), translateVector.get(1), translateVector.get(1));	
		physicsGeometry->getWorldTransform().setOrigin(final);
	}

	nodeTransformation.addTranslation(translateVector);
}

// Scale the node
void SceneNode::scale(float p_sX, float p_sY, float p_sZ)
{
	/* SCALING NOT CONSISTENT BETWEEN PHYSICS AND GRAPHICS
	NEED TO BE FIXED*/

	//if (physicsGeometry != 0) {
	//	btVector3 oldScale = physicsGeometry->getCollisionShape()->getLocalScaling();
	//	physicsGeometry->getCollisionShape()->setLocalScaling(btVector3(oldScale.getX()*p_sX, oldScale.getY()*p_sY, oldScale.getX()*p_sZ));
	//	oldScale = physicsGeometry->getCollisionShape()->getLocalScaling();
	//}

	nodeTransformation.addScaling(p_sX, p_sY, p_sZ);

}

// Apply a shearing to the node
void SceneNode::shear(float p_shXY, float p_shXZ, float p_shYX, float p_shYZ, float p_shZX, float p_shZY)
{
	nodeTransformation.addShearing(p_shXY, p_shXZ, p_shYX, p_shYZ, p_shZX, p_shZY);
}

Vector SceneNode::getWorldPosition(void)
{
	SceneNode * nodePointer = this;
	Vector worldPosition = nodePointer->getTransformation()->getTranslation();
	nodePointer = nodePointer->getParent();
	Quaternion qTemp;


	while(nodePointer->getName() != "root") {
		worldPosition = worldPosition + nodePointer->getTransformation()->getTranslation();

		worldPosition = Matrix::generateQuaternionRotationMatrix(qTemp) * worldPosition;

		nodePointer = nodePointer->getParent();
	}

	return worldPosition;
}

Transformation * SceneNode::getTransformation()
{
	return &nodeTransformation;
}


// Apply the trasformation of the node and then draw it
void SceneNode::drawGeometry()
{
	if (physicsGeometry != 0) {
		btTransform trans;
	
		physicsGeometry->getMotionState()->getWorldTransform(trans);
		Vector local = nodeTransformation.getBBTranslation();
		Vector physic = Vector(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		Vector final = local + physic;
		nodeTransformation.setTranslation(final);

		nodeTransformation.setOrientation(Quaternion(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW()));

		//Vector print = Vector(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		//cout << nodeName << ": " << print << endl;
	}

	applyTransformation();


	if(isVisible()) {
		geometry->drawGeometry();
		// draw the name of the SceneNode
		if(drawDebug) drawName();
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

void SceneNode::drawName(void)
{
	
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);     // need to disable lighting for proper text color

    glColor4f(0.1f, 1.0f, 0.1f, 0.2f);  // set text color
	float pos[3];
	pos[1] = nodeTransformation.getBBTranslation().get(1);
	pos[2] = nodeTransformation.getBBTranslation().get(2);

	char * tempName = (char *)nodeNameString.c_str();
	
	int i = 0;
	// proper starting position
	if(nodeNameString=="boss cube")	i = 15;
	//else i = 150;
	
	// loop all characters in the string
	while(*tempName)
    {
		pos[0] = nodeTransformation.getBBTranslation().get(0)+i;
		glRasterPos3fv(pos);        // place text position
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *tempName);
        ++tempName;
		i+=5;
    }
	
    glEnable(GL_LIGHTING);
    glPopAttrib();

}


// Static member initialization. 
Root Root::_instance;
list<SceneNode*> Root::childOfRootList;
SDL_mutex * Root::rootMutex;
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

