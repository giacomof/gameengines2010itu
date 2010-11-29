#include "extendedEngine.h"

static float const PI = 3.14159f;				// PI definition

void extendedEngine::setupScene()
{
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	assetManager.loadMd2("assets/bosscube.md2", "md2BossCube");

	assetManager.loadTexture("assets/bosscube.jpg", "bossCubeTx");


	// ******************************
	// **** CREATE OBJECTS POINT ****
	// ******************************

	// Create the plane with the collision shape
	btRigidBody * planeRigidBody = this->createPhysicalBox(Vector(1000.0f, 10.0f, 1000.0f), Vector(0.0f, -10.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	SceneObject * plane = this->createPlane(2000.0f, 2000.0f, 100, 0);
	planeNode = this->addSceneNode(&rootNode, "Plane Node", plane, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f);
	// Change BB Translation
	planeNode->getTransformation()->setBBTranslation(Vector(0, 10, 0));



	// Create character "rotation center" node
	SceneObject * sphere = this->createSphere(5, 30, 30, true, Globals::NO_TEXTURE_PHONG);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);
	sphereNode->setVisible(false);

	// Create Light connected it to the battle droid
	Light * light1 = (Light*)this->createLight(true, false, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2);
	light1Node = this->addSceneNode(sphereNode, "Light1 Node", light1, Vector(0, 50, -100), Vector(0, 0, 0), 0);





	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 15.0f, 30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 15.0f, -30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 30.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 30.0f, 30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 30.0f, -30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 45.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 45.0f, 30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	//fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 45.0f, -30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	//fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	




}
void extendedEngine::frameStarted(int frameDelta)
{
	//sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.01f * frameDelta);
	//droidCubeRotationCenter->rotateAboutAxis(Vector(0,1,0),-0.1f * frameDelta);
	//droidCubeNode->rotateAboutAxis(Vector(0,1,0),0.205f * frameDelta);

	sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.1f * frameDelta);
}
void extendedEngine::frameEnded(int frameDelta)
{

}