#include "sceneNode.h"
#include <string>
#include <stdlib.h>
#include <list>

static int nodeCount=0;

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