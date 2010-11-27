#include "extendedEngine.h"

static float const PI = 3.14159f;				// PI definition

void extendedEngine::setupScene()
{
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	assetManager.loadSkyBoxTexture();
	this->addSkyBox(assetManager.getSkyBoxTextureList());

	assetManager.loadMd2("assets/bosscube.md2", "md2BossCube");
	assetManager.loadCollada("assets/duck.dae", "duck");

	assetManager.loadTexture("assets/duckCM.tga", "duckTx");
	//assetManager.loadTexture("assets/map.tga", "duckTx");

	assetManager.createShadingProgram("assets/toon.vert", "assets/toon.frag", "toonShader");
	assetManager.createShadingProgram("assets/shader1.vert", "assets/shader1.frag", "shader1");
	assetManager.createShadingProgram("assets/phong1Point.vert", "assets/phong1Point.frag", "phong1Point");
	assetManager.createShadingProgram("assets/textureShader1.vert", "assets/textureShader1.frag", "textureShader1");
	assetManager.createShadingProgram("assets/textureShader2.vert", "assets/textureShader2.frag", "textureShader2");
	assetManager.createShadingProgram("assets/reflect.vert", "assets/reflect.frag", "reflect");
	//assetManager.activateShadingProgram("toonShader");
	//assetManager.activateShadingProgram("shader1");
	//assetManager.activateShadingProgram("phong1Point");
	//assetManager.activateShadingProgram("textureShader1");
	//assetManager.activateShadingProgram("textureShader2");
	//assetManager.activateShadingProgram("reflect");

	// ******************************
	// **** CREATE OBJECTS POINT ****
	// ******************************
	
	//// Create the plane with the collision shape
	//SceneObject * plane = this->createPlane(2000.0f, 2000.0f, 0);
	//planeNode = this->addSceneNode(&rootNode, "Plane Node", plane, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f);

	// Create character "rotation center" node
	SceneObject * sphere = this->createSphere(5, 30, 30, true);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);
	sphereNode->setVisible(true);

	// Create Light connected to the battle droid
	Light * light1 = (Light*)this->createLight(true, false, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2);
	light1Node = this->addSceneNode(sphereNode, "Light1 Node", light1, Vector(0, 50, -200), Vector(0, 0, 0), 0);



	//// Create Light connected to root
	//Light * light2 = (Light*)this->createLight(true, true, 0.2f,0.2f,0.2f,0.5f,0.5f,0.5f,0.3f,0.3f,0.3f);
	//light2Node = this->addSceneNode(&rootNode, "Light2 Node", light2, Vector(0, 0, 0), Vector(0, 0, 0), 0);
	//light2->setDirection(Vector(0, 1, 0));

	// Create the teapot
	SceneObject * teapot = this->createTeapot(50, false);
	SceneNode * teapotNode = this->addSceneNode(&rootNode, "Teapot Node", teapot, Vector(0.0f, 45.0f, -100.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	//teapotNode->setVisible(false);

	SceneObject * duck = this->createCollada(assetManager.getColladaMesh("duck"), assetManager.getTexture("duckTx"));
	SceneNode * duckNode = this->addSceneNode(&rootNode, "Duck Node", duck, Vector(100.0f, 45.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneObject * sphere2 = this->createSphere(20, 30, 30, false);
	SceneNode * sphereNode = this->addSceneNode(&rootNode, "Sphere Node 2", sphere2, Vector(0, 45, -100), Vector(0, 1, 0), 0);
	sphereNode->setVisible(false);

	//SceneObject * bossCube = this->createMD2(assetManager.getMd2Mesh("md2BossCube"), assetManager.getTexture("bossCubeTx"));
	//SceneNode * bossCubeNode = this->addSceneNode(&rootNode, "Boss Cube Node", bossCube, Vector(100.0f, 45.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

}
void extendedEngine::frameStarted(int frameDelta)
{
	sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.01f * frameDelta);
}
void extendedEngine::frameEnded(int frameDelta)
{

}