#include <DDEngine.h>

class extendedEngine : public DDEngine 
{
public:

	extendedEngine(int screenWidth, int screenHeight, int colorDepth, bool physic) : DDEngine(screenWidth, screenHeight, colorDepth, physic){};

	void setupScene();
	void frameStarted();
	void frameEnded();

	SceneNode * planeNode;
	SceneNode * sphereNode;
	SceneNode * battleDroidNode;
	SceneNode * light1Node;
	SceneNode * light2Node;
	SceneNode * skullNode;
	SceneNode * hornNode1;
	SceneNode * hornNode2;
	SceneNode * droidCubeRotationCenter;
	SceneNode * droidCubeNode;
	SceneNode * fallingCubeNode;
};

