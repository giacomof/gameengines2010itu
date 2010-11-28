#include "extendedEngine.h"

static float const PI = 3.14159f;				// PI definition

void extendedEngine::setupScene()
{
	// ******************************
	// ******** LOADING POINT *******
	// ******************************

	assetManager.loadSkyBoxTexture();
	this->addSkyBox(2500.0f, assetManager.getSkyBoxTextureList());

	assetManager.loadMd2("assets/bosscube.md2", "md2BossCube");
	assetManager.loadCollada("assets/duck.dae", "duck");

	assetManager.loadTexture("assets/bosscube.jpg", "bossCubeTx");
	assetManager.loadTexture("assets/duckCM.tga", "duckTx");
	assetManager.loadTexture("assets/map.tga", "environmentMapTx");

	assetManager.createShadingProgram("assets/finalShader.vert", "assets/finalShader.frag", "finalShader");
	assetManager.activateShadingProgram("finalShader");

	// ******************************
	// **** CREATE OBJECTS POINT ****
	// ******************************
	
	//// Create the plane with the collision shape
	//SceneObject * plane = this->createPlane(2000.0f, 2000.0f, 0);
	//planeNode = this->addSceneNode(&rootNode, "Plane Node", plane, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f);

	// Create character "rotation center" node
	SceneObject * sphere = this->createSphere(5, 30, 30, true, Globals::NO_TEXTURE_PHONG);
	sphereNode = this->addSceneNode(&rootNode, "Rotation Center", sphere, Vector(0, 0, 0), Vector(0, 1, 0), 0);
	sphereNode->setVisible(true);

	// Create Light connected to the battle droid
	Light * light1 = (Light*)this->createLight(true, false, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2);
	light1Node = this->addSceneNode(sphereNode, "Light1 Node", light1, Vector(0, 50, -200), Vector(0, 0, 0), 0);

	SceneObject * sphere2 = this->createSphere(5, 30, 30, false, Globals::NO_TEXTURE_PHONG);
	SceneNode * sphereNode2 = this->addSceneNode(light1Node, "", sphere2, Vector(0, 0, 0), Vector(0, 1, 0), 0);


	// Create the teapot
	SceneObject * greenTeapot = this->createTeapot(50, false, Globals::GREEN);
	SceneNode * greenTeapotNode = this->addSceneNode(&rootNode, "Green Teapot Node", greenTeapot, Vector(0.0f, 45.0f, -0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneObject * phongTeapot = this->createTeapot(50, false, Globals::NO_TEXTURE_PHONG);
	SceneNode * phongTeapotNode = this->addSceneNode(&rootNode, "Phong Teapot Node", phongTeapot, Vector(0.0f, 45.0f, -100.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneObject * specularTeapot = this->createTeapot(50, false, Globals::SPECULAR_MAP, assetManager.getTexture("environmentMapTx"));
	SceneNode * specularTeapotNode = this->addSceneNode(&rootNode, "Specular Teapot Node", specularTeapot, Vector(0.0f, 45.0f, -200.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	
	SceneObject * noShadingDuck = this->createCollada(assetManager.getColladaMesh("duck"), assetManager.getTexture("duckTx"), Globals::TEXTURE_NO_SHADING);
	SceneNode * noShadingDuckNode = this->addSceneNode(&rootNode, "No Shading Duck Node", noShadingDuck, Vector(150.0f, 45.0f,100.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneObject * fixedPipelineShadingDuck = this->createCollada(assetManager.getColladaMesh("duck"), assetManager.getTexture("duckTx"), Globals::TEXTURE_FIXED_PIPELINE);
	SceneNode * fixedPipelineShadingDuckNode = this->addSceneNode(&rootNode, "Fixed Pipeline Shading Duck Node", fixedPipelineShadingDuck, Vector(150.0f, 45.0f,0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneObject * duck = this->createCollada(assetManager.getColladaMesh("duck"), assetManager.getTexture("duckTx"), Globals::TEXTURE_PHONG);
	SceneNode * duckNode = this->addSceneNode(&rootNode, "Duck Node", duck, Vector(150.0f, 45.0f,-100.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneObject * specularDuck = this->createCollada(assetManager.getColladaMesh("duck"), assetManager.getTexture("environmentMapTx"), Globals::SPECULAR_MAP);
	SceneNode * specularDuckNode = this->addSceneNode(&rootNode, "Specular Duck Node", specularDuck, Vector(150.0f, 45.0f, -200.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	

	SceneObject * bossCube = this->createMD2(assetManager.getMd2Mesh("md2BossCube"), Globals::TEXTURE_NO_SHADING, assetManager.getTexture("bossCubeTx"));
	SceneNode * bossCubeNode = this->addSceneNode(&rootNode, "Boss Cube Node", bossCube, Vector(100.0f, 45.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

}
void extendedEngine::frameStarted(int frameDelta)
{
	sphereNode->rotateAboutAxis(Vector(0, 1, 0), 0.01f * frameDelta);
}
void extendedEngine::frameEnded(int frameDelta)
{

}