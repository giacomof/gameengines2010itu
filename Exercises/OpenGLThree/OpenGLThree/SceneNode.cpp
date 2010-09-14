#include "sceneNode.h"
#include <string>
#include <stdlib.h>
#include <list>


#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>

static int nodeCount=0;
root* root::instance_ptr = 0;


SceneNode::SceneNode( SceneNode* pNode, string str ) 
{
	nodeName	= str;
	parentNode	= pNode;
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

	
void SceneNode::addChild( SceneNode pNode ) 
{
	pNode.setParent(this);
	childList.push_back(pNode);
}
		
	
void SceneNode::detachChild( SceneNode* cNode ) 
{
	// setParent root
	// delete the SceneNode from the childList
}

void SceneNode::setParent( SceneNode* cNode ) 
{
	parentNode = cNode;
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

void SceneNode::drawGeometry()
{
}



root::root(void)
{
	nodeName = "root";
	id = nodeCount;
	nodeCount++;

}


root::~root(void)
{
}


root * root::get_instance() 
{
	if (instance_ptr == 0) {
		instance_ptr = new root;
	}
	return instance_ptr;
}

void root::drawGeometry()
{
	glPushMatrix();
    {
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
    }
    glPopMatrix();
}