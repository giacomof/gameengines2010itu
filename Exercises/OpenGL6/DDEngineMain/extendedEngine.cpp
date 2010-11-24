#include "extendedEngine.h"

static float const PI = 3.14159f;				// PI definition

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

	assetManager.loadColladaSkeleton("assets/astroboy.dae", "astroboy_skeleton");

	assetManager.loadTexture("assets/battledroid.png", "battleDroidTx");
	assetManager.loadTexture("assets/lostsoul.jpg", "lostSoulTx");
	assetManager.loadTexture("assets/bosscube.jpg", "bossCubeTx");
	assetManager.loadTexture("assets/duckCM.tga", "duckCM.tga");
	assetManager.loadTexture("assets/boy_10.tga", "boy_10.tga");

	assetManager.createShadingProgram("assets/toonf2.vert", "assets/toonf2.frag", "flatGreenShader");
	//assetManager.activateShadingProgram("flatGreenShader");

	// ******************************
	// **** CREATE OBJECTS POINT ****
	// ******************************

	// Create the plane with the collision shape
	btRigidBody * planeRigidBody = this->createPhysicalBox(Vector(1000.0f, 10.0f, 1000.0f), Vector(0.0f, -10.0f, 0.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	SceneObject * plane = this->createPlane(2000.0f, 2000.0f, 100);
	planeNode = this->addSceneNode(&rootNode, "Plane Node", plane, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f, planeRigidBody);
	// Change BB Translation
	planeNode->getTransformation()->setBBTranslation(Vector(0, 10, 0));


	// Create character "rotation center" node
	SceneObject * sphere = this->createSphere(5, 30, 30, true);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);


	// Create battle droid + bounding box
	btRigidBody * battleDroidRigidBody = this->createPhysicalBox(Vector(20.0f, 25.0f, 20.0f), Vector(0.0f, 25.0f, 500.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	SceneObject * battleDroid = this->createMD2(assetManager.getMd2Mesh("battleDroid"), assetManager.getTexture("battleDroidTx"));
	battleDroidNode = this->addSceneNode(sphereNode, "Battle Droid", battleDroid, Vector(0, 0, 0), Vector(0, 1, 0), 0, battleDroidRigidBody);
	assetManager.getMd2Mesh("battleDroid")->SetAnim(1);


	// Create Light connected to the battle droid
	Light * light1 = (Light*)this->createLight(true, false, 0, 0, 0, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
	light1Node = this->addSceneNode(battleDroidNode, "Light1 Node", light1, Vector(0, 50, 0), Vector(0, 0, 0), 0);

	// Create Light connected to root
	Light * light2 = (Light*)this->createLight(true, true, 0.2f,0.2f,0.2f,0.5f,0.5f,0.5f,0.3f,0.3f,0.3f);
	light2Node = this->addSceneNode(&rootNode, "Light2 Node", light2, Vector(0, 0, 0), Vector(0, 0, 0), 0);
	light2->setDirection(Vector(0, 1, 0));

	// Rotation Center and physics for Skull	
	btRigidBody * skullRigidBody = this->createPhysicalSphere(35.0f, Vector(0.0f, 35.0f, 150.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	SceneObject * skull = this->createMD2(assetManager.getMd2Mesh("md2LostSoul"), assetManager.getTexture("lostSoulTx"));
	skullNode = this->addSceneNode(sphereNode, "Skull Node", skull, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, skullRigidBody);

	// Horns shape and physic
	btCollisionShape * hornBox = this->createCollisionBox(Vector(5.0f, 5.0f, 5.0f));
	btRigidBody * hornBox1 = this->createRigidBody(hornBox, Vector(3.0f, 5.0f, 35.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	SceneObject * hornSphere = this->createSphere(5, 3, 3, true);
	hornNode1 = this->addSceneNode(skullNode, "Horn Node 1", hornSphere, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, hornBox1);
	hornNode1->setVisible(false);

	btRigidBody * hornBox2 = this->createRigidBody(hornBox, Vector(3.0f, 5.0f, -35.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	hornNode2 = this->addSceneNode(skullNode, "Horn Node 2", hornSphere, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, hornBox2);
	hornNode2->setVisible(false);


	// Droid Cube and physic + rotation center
	btCollisionShape * cubeBox = this->createCollisionBox(Vector(10.0f, 10.0f, 10.0f));
	btRigidBody * droidCubeRigidBody = this->createRigidBody(cubeBox, Vector(0.0f, -15.0f, 60.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 0, false);
	SceneObject * droidCube = this->createMD2(assetManager.getMd2Mesh("md2BossCube"), assetManager.getTexture("bossCubeTx"));
	SceneObject * droidCubeRotationSphere = this->createSphere(5, 3, 3, true);
	droidCubeRotationCenter = this->addSceneNode(battleDroidNode, "Droid Cube Rotation Center", droidCubeRotationSphere, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	droidCubeRotationCenter->setVisible(false);
	droidCubeNode = this->addSceneNode(droidCubeRotationCenter, "Droid Cube Node", droidCube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, droidCubeRigidBody);

	// Falling Cube
	btRigidBody * fallingCubeRigidBody = this->createRigidBody(cubeBox, Vector(50.0f, 500.0f, -125.0f), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 10);
	fallingCubeNode = this->addSceneNode(&rootNode, "Falling Cube Node", droidCube, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, fallingCubeRigidBody);	


	// Ducks
	btCollisionShape * duckBox = this->createCollisionBox(Vector(15.0f, 15.0f, 15.0f));
	SceneObject * duck = this->createCollada(assetManager.getColladaMesh("duck"),assetManager.getTexture("duckCM.tga"));

	for(int i = 0; i < 6; i++)
	{
		btRigidBody * duckRigidBody = this->createRigidBody(duckBox, Vector(sin(PI/3*i) * 500 ,350,cos(PI/3*i) * 500), Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 1);
		SceneNode * duckNode = this->addSceneNode(&rootNode, "Duck Node", duck, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, duckRigidBody);
		// Change BB Translation
		duckNode->getTransformation()->setBBTranslation(Vector(-2, -17, 0));
	}


	// Skeletal animated astroboy
	SceneObject * astroboySkeleton = this->createCollada(0,0,assetManager.getColladaSkeleton("astroboy_skeleton"));
	SceneNode * astroboySkeletonNode = this->addSceneNode(&rootNode, "Astroboy Skeleton Node", astroboySkeleton,  Vector(20.0f, 0.0f, 0.0f), Vector(1.0f,0.0f,0.0f), -90.0f);
	astroboySkeletonNode->scale(15.0, 15.0, 15.0);

	// Skeletal animated astroboy with mesh and texture
	SceneObject * astroboyMesh = this->createCollada(assetManager.getColladaMesh("astroboy"),assetManager.getTexture("boy_10.tga"),assetManager.getColladaSkeleton("astroboy_skeleton"));
	SceneNode * astroboyMeshNode = this->addSceneNode(&rootNode, "Astroboy Node", astroboyMesh,  Vector(30.0f, 0.0f, 0.0f), Vector(1.0f,0.0f,0.0f), -90.0f);
	astroboyMeshNode->scale(15.0, 15.0, 15.0);

}
void extendedEngine::frameStarted(int frameDelta)
{
	sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.01f * frameDelta);
	droidCubeRotationCenter->rotateAboutAxis(Vector(0,1,0),-0.1f * frameDelta);
	droidCubeNode->rotateAboutAxis(Vector(0,1,0),0.205f * frameDelta);
}
void extendedEngine::frameEnded(int frameDelta)
{

}