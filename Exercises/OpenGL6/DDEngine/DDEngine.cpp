#include "DDEngine.h"

static int const thread_delay = 1;			// Minimum time between loops
static float const PI = 3.14159f;			// PI definition



DDEngine::DDEngine(int scrW, int scrH, int colorD)
{
	screenWidth = scrW;						// Window Width
	screenHeight = scrH;					// Window Height
	screenColorDepth = colorD;				// Color Depth

	// Create the asset manager
	assetManagerPtr = AssetManager();
	// Create the input pump
	InputPump = MessagePump();
	// Create the window manager
	WindowManager window = WindowManager();
	// Create the input abstraction layer
	controller = Controller();

	// SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		exit(1);
	}

	// Create the window
	window.createWindow(screenWidth, screenHeight, screenColorDepth);

	// Set rendering parameters for OpenGL
	if (initGL()==FALSE)
	{
		
		exit(1);
	}

	// Resize OpenGL window
	window.resizeWindow(screenWidth, screenHeight);

	// Create the root node
	rootNode = Root();
	rootNode.setName("root");

	// Define the exit of the program in relation to SDL variables
	atexit(SDL_Quit);


}


DDEngine::~DDEngine(void)
{

}


int DDEngine::initGL(void)
{
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	// Clears color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// sets the matrix stack as the projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// creates the viewport
	glViewport(0, 0, screenWidth, screenHeight);
	glLoadIdentity();
	// sets the matrix stack as the modelview matrix stack
	glMatrixMode(GL_MODELVIEW);
	gluPerspective(60.0f, screenWidth/screenHeight, 0.1f, 5000.0f);

	// enables the Z-buffer
	glEnable(GL_DEPTH_TEST);
	// enables the texture rendering
	glEnable(GL_TEXTURE_2D);
	// enables smooth shading (garaud)
	glShadeModel(GL_SMOOTH);
	// frontfacing culling
	glEnable(GL_CULL_FACE);
	// enables lighting
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glEnable(GL_COLOR_MATERIAL);
}