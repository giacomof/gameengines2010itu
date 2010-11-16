#include "extendedEngine.h"

void extendedEngine::setupScene()
{
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	assetManager.loadMd2("assets/battledroid.md2", "battleDroid");
	assetManager.loadMd2("assets/Lostsoul.md2", "md2LostSoul");
	assetManager.loadMd2("assets/bosscube.md2", "md2BossCube");
	assetManager.loadCollada("assets/duck.dae", "duck");
	assetManager.loadCollada("assets/astroboy.dae", "astroboy");

	assetManager.loadTexture("assets/battledroid.png", "battleDroidTx");
	assetManager.loadTexture("assets/lostsoul.jpg", "lostSoulTx");
	assetManager.loadTexture("assets/bosscube.jpg", "bossCubeTx");
	assetManager.loadTexture("assets/duckCM.tga", "duckCM.tga");
	assetManager.loadTexture("assets/boy_10.tga", "boy_10.tga");

	//// ******************************
	//// **** CREATE OBJECTS POINT ****
	//// ******************************

	// Create the plane with the collision shape
	btRigidBody * planeRigidBody = this->createPhysicalBox(Vector(1000.0f, 10.0f, 1000.0f), Vector(0.0f, -10.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0);
	SceneObject * plane = this->createPlane(2000.0f, 2000.0f, 100);
	planeNode = this->addSceneNode(&rootNode, "Plane Node", plane, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f, planeRigidBody);
	// Change BB Translation
	planeNode->getTransformation()->setBBTranslation(Vector(0, 10, 0));


	// Create character "rotation center" node
	SceneObject * sphere = this->createSphere(5, 30, 30, true);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);


	// Create battle droid + bounding box
	btRigidBody * battleDroidRigidBody = this->createPhysicalBox(Vector(20.0f, 25.0f, 20.0f), Vector(0.0f, 25.0f, 500.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0);
	SceneObject * battleDroid = this->createMD2(assetManager.getMd2Mesh("battleDroid"), assetManager.getTexture("battleDroidTx"));
	battleDroidNode = this->addSceneNode(sphereNode, "Battle Droid", battleDroid, Vector(0, 0, 0), Vector(0, 1, 0), 0, battleDroidRigidBody);
	assetManager.getMd2Mesh("battleDroid")->SetAnim(1);


	Light * light1 = (Light*)this->createLight(true, false, 0, 0, 0, 1, 1, 1, 1, 1, 1);
	light1Node = this->addSceneNode(battleDroidNode, "Light1 Node", light1, Vector(0, 50, 0), Vector(0, 0, 0), 0);


	Light * light2 = (Light*)this->createLight(true, true, 0.2f,0.2f,0.2f,0.5f,0.5f,0.5f,0.3f,0.3f,0.3f);
	light2Node = this->addSceneNode(&rootNode, "Light2 Node", light2, Vector(0, 0, 0), Vector(0, 0, 0), 0);
	light2->setDirection(Vector(0, 1, 0));




}
void extendedEngine::frameStarted()
{
	sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.2f);

}
void extendedEngine::frameEnded()
{

}