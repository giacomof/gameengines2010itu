#include <DDEngine.h>

class extendedEngine : public DDEngine 
{
public:

	extendedEngine(int screenWidth, int screenHeight, int colorDepth, bool physic) : DDEngine(screenWidth, screenHeight, colorDepth, physic){};

	void setupScene();
	void frameStarted(int frameDelta);
	void frameEnded(int frameDelta);

	SceneNode * planeNode;
	SceneNode * sphereNode;
	SceneNode * fatherSphereSceneNode;
	SceneNode * light1Node;
	SceneNode * fallingCubeNode;
};

