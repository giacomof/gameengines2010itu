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


	SceneObject * sphere = this->createSphere(5, 30, 30, true, Globals::NO_TEXTURE_PHONG);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);
	sphereNode->setVisible(false);


	// Sphere
	btRigidBody * fatherSphereRigidBody = this->createPhysicalSphere(25, Vector(-200,40, 0), Quaternion( 0, 1, 0, 0), 0);
	SceneObject * fatherSphere = this->createSphere(25, 30, 30, false);
	fatherSphereSceneNode = this->addSceneNode(sphereNode, "Father Sphere Node", fatherSphere, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f, fatherSphereRigidBody);

	// Create Light
	Light * light1 = (Light*)this->createLight(true, false, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2);
	light1Node = this->addSceneNode(&rootNode, "Light1 Node", light1, Vector(0, 50, -100), Vector(0, 0, 0), 0);

	


	btCollisionShape * cubeBox = this->createCollisionBox(Vector(10.0f, 10.0f, 10.0f));
	SceneObject * cube = this->createMD2(assetManager.getMd2Mesh("md2BossCube"), assetManager.getTexture("bossCubeTx"));

	// Falling Cube
	btRigidBody * fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 15.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	SceneNode * fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 15.0f, 30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 15.0f, -30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 50.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 50.0f, 30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 50.0f, -30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 75.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 75.0f, 30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	

	fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, 75.0f, -30.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", cube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	


}
void extendedEngine::frameStarted(int frameDelta)
{
	sphereNode->translate(Vector(0.1f, 0, 0) * frameDelta);
}
void extendedEngine::frameEnded(int frameDelta)
{

}