#include <DDEngine.h>

class extendedEngine : public DDEngine 
{
public:

	extendedEngine(int screenWidth, int screenHeight, int colorDepth, bool physic) : DDEngine(screenWidth, screenHeight, colorDepth, physic){};

	void setupScene();
	void frameStarted();
	void frameEnded();

};

