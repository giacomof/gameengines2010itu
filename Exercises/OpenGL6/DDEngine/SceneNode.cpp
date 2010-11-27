#include "sceneNode.h"

static int nodeCount = 0;

// Constructor that take a pointer to the parant node, the name of the node
// the initial position of the node
// the initial orientation of the node
SceneNode::SceneNode(	SceneNode * parentNode, char * str, SceneObject * g,
						Vector v,
						Vector p_axis, float p_angle,
						btRigidBody * rigidBody )
{
	// variables inizialization
//	assetManager = new AssetManager();
	nodeNameString = str;
	parentNode = parentNode;
	parentNode->addChild(this);
	geometry = g;
	physicsGeometry = rigidBody;
	visible = true;
	shadingIsActive = false;
	tranM = new float[16];
	btTransform trans = btTransform();
	mutex_node = SDL_CreateMutex();

	// Initialise the member transformation with the position and orientation parameters
	nodeTransformation = Transformation(v, p_axis, p_angle);

	// Define the unique identifier of the node
	id = nodeCount;
	nodeCount++;

}

// If the geometry is a MD2 model than update his animation status
// and do so for all his childs
void SceneNode::update(float dt) 
{
	//std::cout << "UPDATING: " << nodeNameString << endl;
	if(nodeNameString != "Camera Node") geometry->update();

	if (childList.size() != 0) {

		list<SceneNode*>::iterator itS(childList.begin());
		for(itS = childList.begin(); itS != childList.end(); itS++) {
				(*itS)->update(dt);		
		}

	}
}

void SceneNode::destroy(void) 
{	
	list<SceneNode*>::iterator i(childList.begin());
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
	nodeNameString = name;
}

// Return the name of the node
std::string SceneNode::getName(void) 
{
	if(nodeNameString == "") return "No Name";
	return nodeNameString;
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

			btTransform trans = btTransform();
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
	Vector worldPosition = Vector();
	Vector temp = Vector();

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
	Quaternion temp = Quaternion();

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
	
	//std::cout << "APPLY TRANSFORMATION: " << nodeNameString << endl;
	applyTransformation();

	if(isVisible()) {
		//std::cout << "RENDERING: " << nodeNameString << endl;
		if(nodeNameString != "Camera Node") 
		{
			if(shadingIsActive)
			{
				AssetManager::activateShadingProgram(shadingProgramName);
				geometry->drawGeometry();
				AssetManager::deactivateShadingProgram();
			}
			else
				geometry->drawGeometry();
		}
		// draw the name of the SceneNode
		if(drawDebug) drawName();
	}

	

	if(childList.size() != 0) {

		list<SceneNode*>::iterator itS (childList.begin());
		for(itS = childList.begin(); itS != childList.end(); itS++) {
			(*itS)->drawGeometry();
		}
	}

	glPopMatrix();
}

// Apply the transformation of the node
void SceneNode::applyTransformation()
{
	nodeTransformation.getTransformation().getMatrix(&tranM[0]);
	glPushMatrix();
	glMultMatrixf(&tranM[0]);
}

// Apply the inverse transformation of the node
void SceneNode::removeTransformation()
{
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
	AssetManager::setShaderFlag(GREEN);
	glDisable(GL_LIGHTING);     // need to disable lighting for proper text color

    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);  // set text color
	float * pos = new float[3];
	pos[1] = 0;
	pos[2] = 0;

	char * tempName = (char *)nodeNameString.c_str();
	
	int i = 0;
	// loop all characters in the string
	while(*tempName)
    {
		pos[0] = i;
		glRasterPos3fv(pos);        // place text position
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *tempName);
        ++tempName;
		i+=6;
    }
	
    glEnable(GL_LIGHTING);
}

void SceneNode::setShadingProgram(char * programName)
{
	shadingProgramName = programName;
	shadingIsActive = true;
}

void SceneNode::activateShadingProgram(bool active)
{
	shadingIsActive = active;
}

// Static member initialization. 
Root Root::_instance;
list<SceneNode*> Root::childOfRootList;
SDL_mutex * Root::rootMutex;
unsigned int Root::id;

Root &Root::getInstance()
{
	return _instance;
}

// Method for drawing the entire world moving along the scene graph
void Root::drawGeometry()
{
	list<SceneNode*>::iterator itS(childOfRootList.begin());

	for(itS = childOfRootList.begin(); itS != childOfRootList.end(); itS++) {
			(*itS)->drawGeometry();
	}

	glPopMatrix();
}

// Update the transformation status of all the child nodes of root
void Root::update(float dt)
{
	list<SceneNode*>::iterator itS(childOfRootList.begin());

	for(itS = childOfRootList.begin(); itS != childOfRootList.end(); itS++) {
			(*itS)->update(dt);
			
	}
}

