#include "extendedEngine.h"

static float const PI = 3.14159f;				// PI definition

void extendedEngine::setupScene()
{
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	assetManager.loadMd2("assets/bosscube.md2", "md2BossCube");
	assetManager.loadCollada("assets/duck.dae", "duck");

	assetManager.loadTexture("assets/bosscube.jpg", "bossCubeTx");
	assetManager.loadTexture("assets/duckCM.tga", "duckCM.tga");

	//assetManager.createShadingProgram("assets/toon.vert", "assets/toon.frag", "toonShader");
	assetManager.createShadingProgram("assets/shader1.vert", "assets/shader1.frag", "shader1");
	assetManager.activateShadingProgram("shader1");
	//assetManager.activateShadingProgram("toonShader");

	// ******************************
	// **** CREATE OBJECTS POINT ****
	// ******************************

	// Create the plane with the collision shape
	SceneObject * plane = this->createPlane(2000.0f, 2000.0f, 0);
	planeNode = this->addSceneNode(&rootNode, "Plane Node", plane, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f);

	// Create character "rotation center" node
	SceneObject * sphere = this->createSphere(5, 30, 30, true);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);

	// Create Light connected to the battle droid
	Light * light1 = (Light*)this->createLight(true, false, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	light1Node = this->addSceneNode(sphereNode, "Light1 Node", light1, Vector(0, 100, -400), Vector(0, 0, 0), 0);



	//// Create Light connected to root
	//Light * light2 = (Light*)this->createLight(true, true, 0.2f,0.2f,0.2f,0.5f,0.5f,0.5f,0.3f,0.3f,0.3f);
	//light2Node = this->addSceneNode(&rootNode, "Light2 Node", light2, Vector(0, 0, 0), Vector(0, 0, 0), 0);
	//light2->setDirection(Vector(0, 1, 0));

	// Create the teapot
	SceneObject * teapot = this->createTeapot(50, false);
	SceneNode * teapotNode = this->addSceneNode(&rootNode, "Teapot Node", teapot, Vector(0.0f, 45.0f, -100.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	//teapotNode->setShadingProgram("toonShader");
	//teapotNode->setShadingProgram("shader1");

}
void extendedEngine::frameStarted(int frameDelta)
{
	sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.01f * frameDelta);
}
void extendedEngine::frameEnded(int frameDelta)
{

}