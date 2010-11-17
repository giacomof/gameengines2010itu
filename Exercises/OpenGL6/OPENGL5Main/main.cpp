// Used for checking memory leaks
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>					// Header File for SDL thread library
#include <SDL_audio.h>					// Header File for SDL audio library
#include <glut.h>						// Header File for glut commands
#include <btBulletDynamicsCommon.h>		// Header File for Bullet Physics

#include "memoryManager.h"				// Header File for our Memory Manager
#include "linearAlgebraDLL.h"			// Header File for our math library
#include "sceneNode.h"					// Header File for the SceneNode/Scenegraph
#include "sceneObject.h"				// Header File for the SceneObject container
#include "md2File.h"					// Header File for our md2 loader
#include "colladaFile.h"				// Header File for out COLLADA loader
#include "assetManager.h"				// Header File for our Asset Manager
#include "inputManager.h"				// Header File for our Input Manager
#include "frameClock.h"					// Header File for our Clock
#include "sound.h"						// Header File for our Sound
#include "windowManager.h"				// Header File for our Window Manager
#include "debugDraw.h"					// Header File for our Debug Drawer 

using namespace std;
using namespace linearAlgebraDLL;

static int const screenWidth=800;		// Window Width
static int const screenHeight=600;		// Window Height
static int const screenColorDepth=32;	// Color Depth

static int const tick = 16;				// Minimum time between screen frames
static int const thread_delay = 1;		// Minimum time between loops
static float const PI = 3.14159f;		// PI definition

Uint32 tickFrame = 0;

// Message pumps used for passing Events between threads
MessagePump InputPump;

// Pointer to SDL rendering surface
SDL_Surface * surface;				
GLuint image;

// Root node and other Scene Node
Root * rootNodePtr;
// Asset manager
AssetManager * assetManagerPtr;

btDiscreteDynamicsWorld * dynamicsWorld;

// DebugDrawer Declaration
DebugDraw debugger;

// SceneNodes Declaration
SceneNode * battleDroid;
SceneNode * lostSoul;
SceneNode * bossCube;
SceneNode * colladaDuck;
SceneNode * colladaAstroboy;
SceneNode * testPlane;

// STD/OpenGL Methods
void drawGL(int frameDelta);
int initGL(void);

// Pointer to the function that moves the camera
float* getCamera(void);

// input manager definition
inputManager input;
Controller controller;

// Window Manager definition
WindowManager window = WindowManager();

// Define Test Lights Attributes
GLfloat Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};  
GLfloat Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 

// SDL Mutex for Thread Synchronized access to memory
static SDL_mutex * mutex_allocator = SDL_CreateMutex();

/* ********************************************* */
/* ******* operator new global overload ******** */
/* ******** just remember to use flags ********* */
/* ************* from globals.h **************** */
/* ********************************************* */

void * operator new(size_t size, unsigned short flag)
{
	AssetManager::lockMutex(mutex_allocator);

	cout << "NEW WITH FLAG: " << flag << endl;

	void * storage = MemoryManager::allocate(size);
	if(NULL == storage) {
            throw "allocation fail : no free memory";
    }
	AssetManager::unlockMutex(mutex_allocator);

	return storage;
}

void operator delete(void * ptr) 
{
	free(ptr);
}

void operator delete(void * ptr, unsigned short flag) 
{
	cout << "DELETE WITH FLAG: " << flag << endl;
	MemoryManager::freeToLastMarker();
}

/* This thread handles input */
int threadInput(void *data)
{
	input = input.getInstance();
	char *tname = ( char * )data;

	// Disable the Windows Cursor
	SDL_ShowCursor(SDL_DISABLE); 
	
	// Binds mouse and keyboard input to the OpenGL window
	SDL_WM_GrabInput(SDL_GRAB_ON); 

	while ( !controller.quit ) {
		input.update();

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	SDL_ShowCursor(SDL_ENABLE);

	return 0;
}

///* This thread handles audio */
int threadSound(void *data)
{
	char *tname = ( char * )data;

	soundInit();

	int testsoundint = 500;
	while ( !controller.quit )
	{
		testsoundint++;
		if (testsoundint > 600)
		{
			soundPlayFile("assets/MENULOOP.WAV");
			testsoundint = 0;
		}
		SDL_Delay(thread_delay);
	}

	soundExit();

	return 0;
}

/* This thread updates the scene */
int threadUpdate(void *data)
{
	// Thread name
	char *tname = ( char * )data;

	while ( !controller.quit ) {
		// Runs the update method here

		// physics simulation
		dynamicsWorld->stepSimulation(1/120.f, 10);

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	return 0;
}

// *************************************************************************
// ******************* /* Application entry point */ ***********************
// *************************************************************************

int main(int argc, char *argv[])
{
	
	// Used for checking memory leaks
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 920;


	// Mandatory Initializations
	MemoryManager memMgr = MemoryManager();
	assetManagerPtr		 = new(MANAGER) AssetManager();
	InputPump			 = MessagePump::getInstance();
	WindowManager window = WindowManager();
	controller			 = Controller();

	// SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		exit(1);
	}

	window.createWindow(screenWidth, screenHeight, screenColorDepth);

	// OpenGL Specific Initializations
	if (initGL()==FALSE)
	{		
		exit(1);
	}
	
	// resizes OpenGL window
	window.resizeWindow(screenWidth, screenHeight);

	// Create the root node
	rootNodePtr = new Root();
	rootNodePtr->setName("root");

	// Define the exit of the program in relation to SDL variables
	atexit(SDL_Quit);

	// Initialise the physic world
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

	// initialise the debugDrawer
	debugger.setDebugMode( btIDebugDraw::DBG_DrawWireframe ); 
	dynamicsWorld->setDebugDrawer(&debugger); 
	

	//* ---------------------------------------- *
	// * Graph and asset testing stuff start here *
	// * ---------------------------------------- */


	//* ----------------------------------------- *
	// * Plane with the collision shape			  *
	// * ---------------------------------------- */
	
	// Create the plane with the collision shape
	btCollisionShape* groundShape = new btBoxShape(btVector3(1000.0f, 10.0f, 1000.0f));

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),btVector3(0.0f, -10.0f, 0.0f)));

	btScalar groundMass = 0;
	btVector3 grounInertia(0,0,0);
	groundShape->calculateLocalInertia(groundMass,grounInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(groundMass, groundMotionState, groundShape, grounInertia);
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	dynamicsWorld->addRigidBody(groundRigidBody);

	Plane testPlaneGeom(2000.0f, 2000.0f, 200);
	testPlane = new SceneNode(rootNodePtr, "test plane", &testPlaneGeom, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f, groundRigidBody);
	testPlane->getTransformation()->setBBTranslation(Vector(0, 10, 0));
	testPlane->setVisible(true);



	//* ----------------------------------------- *
	// * Rotation Center for battleDroid		  *
	// * ---------------------------------------- */
	// Create character "rotationabout" node
	Sphere rotation_sphere = Sphere(5, 30, 30, true);
	SceneNode rotationCenter(rootNodePtr, "rotationCenter", &rotation_sphere, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	rotationCenter.setVisible(true);


	btCollisionShape* droidBB = new btBoxShape(btVector3(20.0f, 25.0f, 20.0f));

	btDefaultMotionState* droidMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(0,25,500)));

	btScalar droidMass = 0;
	btVector3 droidInertia(0,0,0);
	droidBB->calculateLocalInertia(droidMass,droidInertia);

	btRigidBody::btRigidBodyConstructionInfo battleDroidRigidBodyCI(droidMass, droidMotionState, droidBB, droidInertia);

	btRigidBody* battleDroidRigidBody = new btRigidBody(battleDroidRigidBodyCI);
	dynamicsWorld->addRigidBody(battleDroidRigidBody);

	md2Interface battleDroidMesh = md2Interface(assetManagerPtr->getMd2Mesh("battleDroid"), assetManagerPtr->getTexture("battleDroidTx"));
	battleDroid = new SceneNode(&rotationCenter, "Battle Droid", &battleDroidMesh, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, battleDroidRigidBody);
	battleDroid->setVisible(true);
	assetManagerPtr->getMd2Mesh("battleDroid")->SetAnim(1);



	//* ----------------------------------------- *
	// * Lights Connected to the battleDroid and to root		  *
	// * ---------------------------------------- */
	Light testLight = Light();
	Light testLight1 = Light(true, true, 0.2f,0.2f,0.2f,0.5f,0.5f,0.5f,0.3f,0.3f,0.3f);
	testLight1.setDirection(Vector(0, -1, 0));

	SceneNode * testLightNode = new SceneNode(battleDroid, "Light Node", &testLight, Vector(0.0f, 50.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	SceneNode * testLightNode1 = new SceneNode(rootNodePtr, "Light Node", &testLight1, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);




	//* ----------------------------------------- *
	// * Rotation Center and physics for Skull	  *
	// * ---------------------------------------- */
	btCollisionShape* skullShape = new btSphereShape(35.0f);

	btDefaultMotionState* skullMotionState;

	btScalar skullMass = 0;
	btVector3 skullInertia(0,0,0);
	skullShape->calculateLocalInertia(skullMass,skullInertia);

	btRigidBody::btRigidBodyConstructionInfo * skullRigidBodyCI;

	btRigidBody* skullRigidBody;

	
	skullMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(0,35,150)));
	skullRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(skullMass,skullMotionState,skullShape,skullInertia);

	skullRigidBody = new btRigidBody(*skullRigidBodyCI);


	dynamicsWorld->addRigidBody(skullRigidBody);


	md2Interface lostSoul_g = md2Interface(assetManagerPtr->getMd2Mesh("md2LostSoul"), assetManagerPtr->getTexture("lostSoulTx"));
	lostSoul = new SceneNode(&rotationCenter, "LostSoul", &lostSoul_g, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, skullRigidBody);

	//* -----------------------------------------         *
	// * SceneNode and physics for Skull's horns	      *
	// * ----------------------------------------         */
	btCollisionShape* hornShape = new btBoxShape(btVector3(5.0f, 5.0f, 5.0f));

	btDefaultMotionState* hornMotionState;

	btScalar hornMass = 0;
	btVector3 hornInertia(0,0,0);
	skullShape->calculateLocalInertia(hornMass,hornInertia);

	btRigidBody::btRigidBodyConstructionInfo * hornRigidBodyCI;

	btRigidBody* hornRigidBody;

	
	hornMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(3,5,35)));
	hornRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(hornMass,hornMotionState,hornShape,hornInertia);

	hornRigidBody = new btRigidBody(*hornRigidBodyCI);


	dynamicsWorld->addRigidBody(hornRigidBody);

	Sphere hornSphere1 = Sphere(5, 3, 3, true);
	SceneNode hornNode1(lostSoul, "hornNode", &hornSphere1, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, hornRigidBody);
	hornNode1.setVisible(false);
	

	//* -----------------------------------------         *
	// * SceneNode and physics for Skull's horns	      *
	// * ----------------------------------------         */

	hornMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(3,5,-35)));
	hornRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(hornMass,hornMotionState,hornShape,hornInertia);

	hornRigidBody = new btRigidBody(*hornRigidBodyCI);


	dynamicsWorld->addRigidBody(hornRigidBody);

	Sphere hornSphere2 = Sphere(5, 3, 3, true);
	SceneNode hornNode2(lostSoul, "hornNode", &hornSphere1, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, hornRigidBody);
	hornNode2.setVisible(false);





	//* -----------------------------------------         *
	// * SceneNodes and physics for the droid cube	      *
	// * ----------------------------------------         */

	btCollisionShape* cubeShape = new btBoxShape(btVector3(10.0f, 10.0f, 10.0f));

	btDefaultMotionState* cubeMotionState;

	btScalar cubeMass = 0;
	btVector3 cubeInertia(0,0,0);
	cubeShape->calculateLocalInertia(cubeMass,cubeInertia);

	btRigidBody::btRigidBodyConstructionInfo * cubeRigidBodyCI;

	btRigidBody* cubeRigidBody;

	
	cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(0,-15,60)));
	cubeRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(cubeMass,cubeMotionState,cubeShape,cubeInertia);

	cubeRigidBody = new btRigidBody(*cubeRigidBodyCI);

	dynamicsWorld->addRigidBody(cubeRigidBody);


	md2Interface * bossCube_g = new md2Interface(assetManagerPtr->getMd2Mesh("md2BossCube"), assetManagerPtr->getTexture("bossCubeTx"));
	Sphere rotationSphere2 = Sphere(5, 3, 3, true);
	SceneNode rotationNode2(battleDroid, "rotationNode2", &hornSphere1, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	rotationNode2.setVisible(false);
	bossCube = new SceneNode(&rotationNode2, "boss cube", bossCube_g, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, cubeRigidBody);



	//* -----------------------------------------         *
	// * SceneNodes and physics for falling cube	      *
	// * ----------------------------------------         */
	cubeMass = 10;

	cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(50,500,-125)));
		
	cubeRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(cubeMass,cubeMotionState,cubeShape,cubeInertia);

	cubeRigidBody = new btRigidBody(*cubeRigidBodyCI);

	cubeRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(cubeRigidBody);

	bossCube_g = new md2Interface(assetManagerPtr->getMd2Mesh("md2BossCube"), assetManagerPtr->getTexture("bossCubeTx"));
	SceneNode * bossCube2 = new SceneNode(rootNodePtr, "falling boss cube", bossCube_g, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, cubeRigidBody);





	//* -----------------------------------------         *
	// * SceneNodes and physics for ducks	      *
	// * ----------------------------------------         */
	btCollisionShape* duckShape = new btBoxShape(btVector3(15.0f, 15.0f, 15.0f));

	btDefaultMotionState* duckMotionState;

	btScalar duckMass = 1;
	btVector3 duckInertia(0,0,0);
	duckShape->calculateLocalInertia(duckMass,duckInertia);

	btRigidBody::btRigidBodyConstructionInfo * duckRigidBodyCI;

	btRigidBody* duckRigidBody;

	
	ColladaInterface duck_g = ColladaInterface(assetManagerPtr->getColladaMesh("duck"), assetManagerPtr->getTexture("duckCM.tga"), 0);



	for(int i = 0; i < 6; i++)
	{

		duckMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,0,1),0.0f),btVector3(sin(PI/3*i) * 500 ,350,cos(PI/3*i) * 500)));
		duckRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(duckMass,duckMotionState,duckShape,duckInertia);

		duckRigidBody = new btRigidBody(*duckRigidBodyCI);

		duckRigidBody->setActivationState(DISABLE_DEACTIVATION);

		dynamicsWorld->addRigidBody(duckRigidBody);


		colladaDuck = new SceneNode(rootNodePtr, "duck", &duck_g,  Vector(0.0f, 10.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, duckRigidBody);
		colladaDuck->getTransformation()->setBBTranslation(Vector(-2, -17, 0));

	}



	colladaDuck = new SceneNode(rootNodePtr, "duck", &duck_g,  Vector(0.0f, 10.0f, 0.0f), Vector(25.0f,0.0f,25.0f), 0.0f);

	ColladaInterface astroboy_g = ColladaInterface(assetManagerPtr->getColladaMesh("astroboy"), assetManagerPtr->getTexture("boy_10.tga"), assetManagerPtr->getColladaSkeleton("astroboy_skeleton"));
	colladaAstroboy = new SceneNode(rootNodePtr, "astroboy", &astroboy_g,  Vector(200.0f, 10.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);


	/* ------------------------------------------ *
	 * Graph and asset testing stuff ends here  *
	 * ---------------------------------------- */

	// Set up camera and spectator
	entitySpectator *player = new entitySpectator();
	entityCamera *playercamera = new entityCamera();
	player->setCamera(playercamera);

	SceneNode * cameraNode = new SceneNode(battleDroid, "Camera Node", playercamera, Vector(-40.0f, 30.0f, 0.0f), Vector(0.0f,1.0f,0.0f), -90.0f );
	cameraNode->rotateAboutAxis(Vector(0,0,1),-20);
	playercamera->setSceneNode(cameraNode);
	playercamera->isFollowingNode = false;

	controller.setPlayerObject(player);

	// Set up fps clock
	frameClock renderClock;

	// Create the thread handles and assign names
	SDL_Thread *id1;
	SDL_Thread *id2;
	SDL_Thread *id3;
	char *tnames[] = { "General Update", "Sound Manager", "Input"};

	// Create the threads
	id1 = SDL_CreateThread ( threadUpdate, tnames[0] );
	id2 = SDL_CreateThread ( threadSound, tnames[1] );
	id3 = SDL_CreateThread ( threadInput, tnames[2] );

	char title[80];
	SDL_Event currentEvent;

	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shine = 100.0;

	glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, ambient[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, specular[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	int frameDelta = 0;

	while(!controller.quit)
	{
		
		frameDelta = renderClock.getFrameDelta();

		sprintf_s(title, "Name Here Engine | %i FPS", renderClock.getFPS() );
		window.setTitle( title, "include/nhe.ico" );
		
		rotationCenter.rotateAboutAxis(Vector(0,1,0),0.01f*frameDelta);
		rotationNode2.rotateAboutAxis(Vector(0,1,0),-0.1f*frameDelta);
		bossCube->rotateAboutAxis(Vector(0,1,0),0.205f*frameDelta);
		bossCube2->rotateAboutAxis(Vector(0,0,1),0.002f*frameDelta);
		bossCube2->translate(Vector(0.01f*frameDelta,0,0));
		

		// Time to take care of the SDL events we have recieved
		while(SDL_PollEvent(&currentEvent))
		{
			switch(currentEvent.type)
			{
			// Some general events to handle immediately
			case SDL_ACTIVEEVENT:
				InputPump.sendMessage(currentEvent);
				if (currentEvent.active.state & SDL_APPACTIVE)
				{
					if (currentEvent.active.gain==0)
						window.setActive(FALSE);
					else
						window.setActive(TRUE);
				}
				break;
			case SDL_VIDEORESIZE:
				surface = SDL_SetVideoMode(	currentEvent.resize.w, currentEvent.resize.h, screenColorDepth, window.getVideoFlags());
				if (!surface)
				{
					delete assetManagerPtr;
					exit(1);
				}
				window.resizeWindow( currentEvent.resize.w, currentEvent.resize.h);
				break;
			case SDL_QUIT:
				controller.quit = true;
				break;
			// Input events coming below. They are all just passed on to the input pump
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				InputPump.sendMessage(currentEvent);
				break;
			// Anything else we don't care about
			default:
				break;
			}
		}

		controller.playerObject->update();
		
		// Actual frame rendering happens here
		if (window.getActive() )
		{
			renderClock.frameUpdate();
			drawGL( frameDelta );
		}		

	}

	//wait for the threads to exit
	SDL_WaitThread ( id1, NULL );
	SDL_WaitThread ( id2, NULL );
	SDL_WaitThread ( id3, NULL );

	exit(0);  
}

/* Draw the scene */
void drawGL(int frameDelta)
{
	// clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glLoadIdentity();

	// Set the camera
	float * CamTransform = getCamera();

	// draw the debug drawings if the engine is in debug mode
	if(drawDebug) dynamicsWorld->debugDrawWorld();

	// update the animations
	rootNodePtr->update(frameDelta);
	// draw everything
	rootNodePtr->drawGeometry();
	
	// Swaps the buffers
	SDL_GL_SwapBuffers();
}

/* Initialize OpenGL */
int initGL(void)
{

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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
	//glCullFace(GL_FRONT);
	// enables lighting
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glEnable(GL_COLOR_MATERIAL);
	
		
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	//assetManagerPtr->loadMd2("include/Cyber.md2", "md2Demon");
	assetManagerPtr->loadMd2("assets/battledroid.md2", "battleDroid");
	assetManagerPtr->loadMd2("assets/Lostsoul.md2", "md2LostSoul");
	assetManagerPtr->loadMd2("assets/bosscube.md2", "md2BossCube");
	assetManagerPtr->loadCollada("assets/duck.dae", "duck");
	assetManagerPtr->loadCollada("assets/astroboy.dae", "astroboy");

	//assetManagerPtr->loadTexture("include/cyber.jpg", "doomDemonTx");
	assetManagerPtr->loadTexture("assets/battledroid.png", "battleDroidTx");
	assetManagerPtr->loadTexture("assets/lostsoul.jpg", "lostSoulTx");
	assetManagerPtr->loadTexture("assets/bosscube.jpg", "bossCubeTx");
	assetManagerPtr->loadTexture("assets/duckCM.tga", "duckCM.tga");
	assetManagerPtr->loadTexture("assets/boy_10.tga", "boy_10.tga");

	//assetManagerPtr->loadColladaSkeleton("include/astroboy.dae", "astroboy_skeleton");

	return TRUE;
}

/* Apply camera matrices */
float* getCamera()
{		
	glMatrixMode(GL_MODELVIEW);

	entityCamera *currentCamera = controller.playerObject->getCamera();

	float tranM[16];

	Matrix::generateIdentityMatrix().getMatrix(&tranM[0]);

	if ( currentCamera != NULL )
	{
		AssetManager::lockMutex( currentCamera->mutex_object );
		Matrix transformationMatrix = Matrix();

		if(currentCamera->isFollowingNode && currentCamera->positionNode != NULL)
		{
			Quaternion orientation = currentCamera->positionNode->getWorldOrientation();
			Vector position = currentCamera->positionNode->getWorldPosition();
			transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(-orientation.getX(), -orientation.getY(), -orientation.getZ(), orientation.getW())).getTranspose();
			transformationMatrix = Matrix::generateTranslationMatrix(-position.getX(), -position.getY(), -position.getZ()).getTranspose() * transformationMatrix;


		} else {

			transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(1.0,0.0,0.0),currentCamera->pitch)).getTranspose();
			transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(0.0,1.0,0.0),currentCamera->yaw)).getTranspose() * transformationMatrix;
			transformationMatrix = Matrix::generateTranslationMatrix(	currentCamera->vPosition[0],
																		currentCamera->vPosition[1],
																		currentCamera->vPosition[2]).getTranspose() * transformationMatrix;			
		}



		transformationMatrix.getMatrix(&tranM[0]);

		AssetManager::unlockMutex( currentCamera->mutex_object );
	}

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}
