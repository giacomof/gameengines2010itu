#include <string>
#include <list>

#include <stdlib.h>
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>

#include "sceneNode.h"

static int nodeCount=0;

// Constructor that take a pointer to the parant node, the name of the node
// the initial position of the node
// the initial orientation of the node
SceneNode::SceneNode(	SceneNode * parentNode, string str, 
						float p_tX, float p_tY, float p_tZ,
						Vector p_axis, float p_angle)
{
	// variables inizialization
	nodeName = str;
	parentNode = parentNode;

	// set parent node
	parentNode->addChild(this);

	// Initialise the member transformation with the position and orientation parameters
	nodeTransformation = Transformation(p_tX, p_tY, p_tZ,
										p_axis, p_angle);
	// Define the unique identifier of the node
	id = nodeCount;
	nodeCount++;
	
}

void SceneNode::update(void) 
{

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
	childList.push_back(cNode);
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

// Apply a rotation about an arbitrary axis to the node
void SceneNode::rotateAboutAxis(Vector p_Axis, float p_Degree)
{
	nodeTransformation.addQuaternionRotation(Quaternion(p_Axis, p_Degree));
}

// Translate the node
void SceneNode::translate(float p_tX, float p_tY, float p_tZ) 
{	
	nodeTransformation.addTranslation(p_tX, p_tY, p_tZ);
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

	/*
	float y = -10.0f;
	glColor3f(0.0f, 1.0f, 0.0f);
	float red, green, blue = 0;
	for(float z = -50; z <= 50; z += 1) {
		green = 0.01 * (50-z);
		for(float x = -50; x <= 50; x += 1) {
			blue = red = 0.01 * (50-x);
			glColor3f(red, green, blue);
			glBegin(GL_TRIANGLES);
				glVertex3f(x+1, y, z);
				glVertex3f(x, y, z+1);
				glVertex3f(x+1, y, z+1);
			glEnd();
		}
    }
	*/
	glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0, 0, 0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(100, 0, 0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0, 0, -100);
	glEnd();
	
	

	list<SceneNode*>::iterator itS;
	for(itS = childList.begin(); itS != childList.end(); itS++) {
			//removeTransformation();
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

void SceneNode::removeTransformation()
{
	float tranM[16];
	

	nodeTransformation.getInverseTransformation().getMatrix(&tranM[0]);
	glPushMatrix();
	glMultMatrixf(&tranM[0]);

}

// Overloaded constructor for the Root node
Root::Root()
{
	nodeName = "Root";
	id = nodeCount;
	nodeCount++;
}


Root::~Root(void)
{
}

// Method for drawing the entire world moving along the scene graph
void Root::drawGeometry()
{
	list<SceneNode*>::iterator itS;

	for(itS = childList.begin(); itS != childList.end(); itS++) {
			(*itS)->drawGeometry();
			
			
	}
	glPopMatrix();
	
}