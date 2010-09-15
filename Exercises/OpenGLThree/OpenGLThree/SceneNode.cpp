#include "sceneNode.h"
#include <string>
#include <stdlib.h>
#include <list>


#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>

static int nodeCount=0;
//Root* Root::instance_ptr = 0;


SceneNode::SceneNode(	SceneNode * pNode, string str, 
						float p_tX,		float p_tY,		float p_tZ,
						float p_angleX, float p_angleY, float p_angleZ ) 
{
	nodeName	= str;
	parentNode	= pNode;
	parentNode->addChild(*this);
	// sets the unique id of the sceneNode
	id = nodeCount;
	nodeCount++;
		
}

void SceneNode::update(void) 
{



}

void SceneNode::destroy(void) 
{
		
	list<SceneNode>::iterator i;
	for(i=childList.begin(); i != childList.end(); ++i) 
	{ 
		i->destroy(); 
	}
		
	this->release();

}

	
void SceneNode::addChild( SceneNode & cNode ) 
{
	cNode.setParent(*this);
	childList.push_back(cNode);
}
		
	
void SceneNode::detachChild( SceneNode & cNode ) 
{
	// setParent Root
	// delete the SceneNode from the childList
}

void SceneNode::setParent( SceneNode & cNode ) 
{
	parentNode = &cNode;
}

SceneNode* SceneNode::getParent(void)
{
	return parentNode;
}

void SceneNode::setName(string name) 
{
	nodeName = name;
}

string SceneNode::getName(void) 
{
	return nodeName;
}

void SceneNode::addTransformation(Transformation & t)
{
	transformationList.add(t);
}

void SceneNode::drawGeometry()
{
	applyTransformation();

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

	glPopMatrix();
}

void SceneNode::applyTransformation()
{
	glPushMatrix();
	float tranM[16];
	Matrix transformationMatrix;
	list<Transformation>::iterator itT;
	list<SceneNode>::iterator itS;
	
	/*
	for(itT = transformationList.begin(); itT != transformationList.end(); ++itT) {
		if ( itT == transformationList.begin() ) {
			transformationMatrix = itT->getTransformation();
		} else {
			transformationMatrix = itT->getTransformation() * transformationMatrix;
		}
	}

	transformationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);
	glPopMatrix();

	for(itS = childList.begin(); itS != childList.end(); ++ itS) {
			itS->drawGeometry();
	}

	*/

}



Root::Root(void)
{
	nodeName = "Root";
	id = nodeCount;
	nodeCount++;

}


Root::~Root(void)
{
}





void Root::drawGeometry()
{
}