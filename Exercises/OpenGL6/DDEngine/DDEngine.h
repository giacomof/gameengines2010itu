#ifdef __DDEngine
#    define DDEngine_D __declspec(dllexport)
#else
#    define DDEngine_D __declspec(dllimport)
#endif

#ifndef DDEngine__H 
#define DDEngine__H

#include <iostream>								// Header File For the basic Input/Output system
#include <windows.h>							// Header File For Windows
#include <stdio.h>								// Header File For Standard Input/Output
#include <stdlib.h>								// Header File For the STD library
#include <SDL.h>								// Header File for the SDL library
#include <SDL_opengl.h>							// Header File for OpenGL through SDL
#include <SDL_thread.h>							// Header File for SDL thread library
#include <SDL_audio.h>							// Header File for SDL audio library
#include <glut.h>								// Header File for glut commands
#include <btBulletDynamicsCommon.h>				// Header File for Bullet Physics

#include "memoryManager.h"						// Header File for our Memory Manager
#include "linearAlgebraDLL.h"					// Header File for our math library
#include "sceneNode.h"							// Header File for the SceneNode/Scenegraph
#include "sceneObject.h"						// Header File for the SceneObject container
#include "md2File.h"							// Header File for our md2 loader
#include "colladaFile.h"						// Header File for out COLLADA loader
#include "assetManager.h"						// Header File for our Asset Manager
#include "inputManager.h"						// Header File for our Input Manager
#include "frameClock.h"							// Header File for our Clock
#include "sound.h"								// Header File for our Sound
#include "windowManager.h"						// Header File for our Window Manager
#include "debugDraw.h"							// Header File for our Debug Drawer 

using namespace std;
using namespace linearAlgebraDLL;

class DDEngine
{
public:

	DDEngine(int screenWidth, int screenHeight, int colorDepth, Vector gravity);
	~DDEngine(void);

	void run();
	void setupScene();
	void frameStarted();
	void frameEnded();

	inline inputManager * getInputManager() { return &input; };
	inline Controller * getController() { return &controller; };
	inline btDiscreteDynamicsWorld * getDynamicWorld() { return dynamicsWorld; };


private:
	
	int screenW;								// Window Width
	int screenH;								// Window Height
	int screenCD;								// Color Depth
	Vector physicGravity;

	// Message pumps used for passing Events between threads
	MessagePump InputPump;
	// Pointer to SDL rendering surface
	SDL_Surface * surface;				
	GLuint image;
	// Root node and other Scene Node
	Root rootNode;
	// Asset manager
	AssetManager assetManagerPtr;
	// Debug Drawing system
	DebugDraw debugger;
	// Input manager and Controller
	inputManager input;
	Controller controller;
	// Window Manager definition
	WindowManager window;
	// Start the MemoryManager
	MemoryManager memMgr;

	// Physic world
	btDiscreteDynamicsWorld * dynamicsWorld;
	// Broadphase
	btDbvtBroadphase broadphase;
	// Collision Configuration
    btDefaultCollisionConfiguration collisionConfiguration;
	// Collision Dispatcher
    btCollisionDispatcher * dispatcher;
	// Constraint Solver
    btSequentialImpulseConstraintSolver solver;

	// FPS Clock
	frameClock renderClock;


	int initGL(void);							// Initialise the rendering window
	int initPhysics(void);						// Initialise the physic engine
	void drawGL(int frameDelta);				// Draw the world
	float * getCamera(void);					// Move the camera
};

#endif;