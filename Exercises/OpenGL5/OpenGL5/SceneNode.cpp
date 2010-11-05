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

	list<SceneNode*>::iterator itS;
	for(itS = childList.begin(); itS != childList.end(); itS++) {
			(*itS)->update(dt);		
	}
}

void SceneNode::destroy(void) 
{	
	list<SceneNode*>::iterator i;
	for(i=childList.begin(); i != childList.end(); ++i) 
	{ 
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
	if (physicsGeometry != 0) 
	{
		if (this->getParent()->getName() == "root")
		{

			btTransform trans;
			physicsGeometry->getMotionState()->getWorldTransform(trans);

			btVector3 btActualPosition = physicsGeometry->getWorldTransform().getOrigin();
			Vector actualPosition = Vector(btActualPosition.getX(), btActualPosition.getY(), btActualPosition.getZ());
			Vector newPosition = Matrix::generateAxisRotationMatrix(p_Axis, p_Degree) * actualPosition;
			physicsGeometry->getWorldTransform().setOrigin(btVector3(newPosition.getX(), newPosition.getY(), newPosition.getZ()));

			btQuaternion actualRotation = trans.getRotation();
			btQuaternion newRotation = btQuaternion(btVector3(p_Axis.getX(), p_Axis.getY(), p_Axis.getZ()), p_Degree);
			physicsGeometry->getWorldTransform().setRotation(newRotation * actualRotation);
		}
	}

	nodeTransformation.addQuaternionRotation(Quaternion(p_Axis, p_Degree));
}

// Translate the node
void SceneNode::translate(Vector translateVector) 
{	
	if (physicsGeometry != 0) 
	{
		if (this->getParent()->getName() == "root")
		{
			btTransform trans;
			physicsGeometry->getMotionState()->getWorldTransform(trans);
			btVector3 final = trans.getOrigin() + btVector3(translateVector.getX(), translateVector.getY(), translateVector.getZ());	
			physicsGeometry->getWorldTransform().setOrigin(final);
		}
	}

	nodeTransformation.addTranslation(translateVector);
}

void SceneNode::updateRigidBody(void) 
{	
	Vector worldPosition = this->getWorldPosition();
	Quaternion worldOrientation = this->getWorldOrientation();
	physicsGeometry->getWorldTransform().setOrigin(btVector3(worldPosition.getX(), worldPosition.getY(), worldPosition.getZ()));
	physicsGeometry->getWorldTransform().setRotation(btQuaternion(worldOrientation.getX(), worldOrientation.getY(), worldOrientation.getZ(), worldOrientation.getW()));
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
	Vector worldPosition;
	Vector temp;

	while(true)
	{
		temp = nodePointer->getTransformation()->getTranslation();

		if(nodePointer->getParent()->getName() == "root")
		{
			worldPosition = worldPosition + temp;
			break;
		}
		else
		{
			nodePointer = nodePointer->getParent();
			worldPosition = worldPosition + temp;
			worldPosition = Matrix::generateQuaternionRotationMatrix(nodePointer->getTransformation()->getOrientation()) * worldPosition;
		}
	}

	return worldPosition;
}

Quaternion SceneNode::getWorldOrientation(void)
{
	SceneNode * nodePointer = this;
	Quaternion worldOrientation = Quaternion(0,0,0,1);
	Quaternion temp;

	worldOrientation = this->getTransformation()->getOrientation() * worldOrientation;

	while(nodePointer->getParent()->getName() != "root")
	{
		temp = nodePointer->getTransformation()->getOrientation();

		nodePointer = nodePointer->getParent();
		temp = nodePointer->getTransformation()->getOrientation();
		worldOrientation = temp * worldOrientation;
		
	}
	return worldOrientation;
}

Transformation * SceneNode::getTransformation()
{
	return &nodeTransformation;
}


// Apply the transformation of the node and then draw it
void SceneNode::drawGeometry()
{

	if (physicsGeometry != 0) {

		if (this->getParent()->getName() == "root")
		{
			btTransform trans;
			physicsGeometry->getMotionState()->getWorldTransform(trans);

			Vector localPosition = nodeTransformation.getBBTranslation();
			Vector physicPosition = Vector(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

			Quaternion orientation = Quaternion(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW());
			localPosition = Matrix::generateQuaternionRotationMatrix(orientation) * localPosition;

			Vector finalPosition = localPosition + physicPosition;
			nodeTransformation.setTranslation(finalPosition);

			nodeTransformation.setOrientation(Quaternion(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW()));
		}
		else
		{
			btTransform trans;
			physicsGeometry->getMotionState()->getWorldTransform(trans);

			Vector localPosition = nodeTransformation.getBBTranslation();

			Quaternion orientation = Quaternion(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ(), trans.getRotation().getW());
			localPosition = Matrix::generateQuaternionRotationMatrix(orientation) * localPosition;

			Vector physicPosition = Vector(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			Vector finalPosition = localPosition + physicPosition;
			nodeTransformation.setTranslation(finalPosition);

			this->updateRigidBody();
		}
	}
	
	applyTransformation();

	if(isVisible()) {
		geometry->drawGeometry();
		// draw the name of the SceneNode
		if(drawDebug) drawName();
	}

	list<SceneNode*>::iterator itS;
	for(itS = childList.begin(); itS != childList.end(); itS++) {
			(*itS)->drawGeometry();
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
void SceneNode::setVisible(bool b) 
{
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
	pos[1] = 0;
	pos[2] = 0;

	char * tempName = (char *)nodeNameString.c_str();
	
	int i = 0;
	// proper starting position
	if(nodeNameString=="boss cube")	i = 15;
	//else i = 150;
	
	// loop all characters in the string
	while(*tempName)
    {
		pos[0] = i;
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
		//AssetManager::lockMutex( rootMutex );
			(*itS)->drawGeometry();
		//AssetManager::unlockMutex( rootMutex );
	}

	glPopMatrix();
}

// Update the transformation status of all the child nodes of root
void Root::update(float dt)
{
	list<SceneNode*>::iterator itS;

	for(itS = childOfRootList.begin(); itS != childOfRootList.end(); itS++) {
			//AssetManager::lockMutex( rootMutex );
			(*itS)->update(dt);
			//AssetManager::unlockMutex( rootMutex );				
	}
}

