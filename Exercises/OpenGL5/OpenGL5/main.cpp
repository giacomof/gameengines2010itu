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
static int const thread_delay = 3;		// Minimum time between loops
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
SceneNode * testPlane;

// OpenGL Attributes
Uint32 timeLeft(void);
GLuint loadTexture(char* texName);
int initGL(void);

// STD/OpenGL Methods
void drawGL(void);

// Pointer to the function that moves the camera
float* getCamera();

GLuint	filter;
GLuint	texture[3];

// input manager definition
inputManager input;

// Window Manager definition
WindowManager window = WindowManager();

// Define Test Lights Attributes
GLfloat Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};  
GLfloat Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 

/* This thread handles input */
int threadInput(void *data)
{
	input = input.getInstance();
	char *tname = ( char * )data;

	// Disable the Windows Cursor
	SDL_ShowCursor(SDL_DISABLE); 
	
	// Binds mouse and keyboard input to the OpenGL window
	SDL_WM_GrabInput(SDL_GRAB_ON); 

	while ( !Controller::quit ) {
		input.update();

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	SDL_ShowCursor(SDL_ENABLE);

	return 0;
}

/* This thread handles audio */
int threadSound(void *data)
{
	char *tname = ( char * )data;

	soundInit();

	int testsoundint = 500;
	while ( !Controller::quit )
	{
		testsoundint++;
		if (testsoundint > 600)
		{
			soundPlayFile("include/MENULOOP.WAV");
			testsoundint = 0;
		}
		SDL_Delay(tick);
	}

	soundExit();

	return 0;
}

/* This thread updates the scene */
int threadUpdate(void *data)
{
	// Thread name
	char *tname = ( char * )data;

	while ( !Controller::quit ) {
		// Runs the update method here

		// physics simultation
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


	// start the asset manager
	assetManagerPtr = new AssetManager();
	InputPump = MessagePump::getInstance();
	WindowManager window = WindowManager();

	// SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		exit(1);
	}

	window.createWindow(screenWidth, screenHeight, screenColorDepth);

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
	// set debugger
	dynamicsWorld->setDebugDrawer(&debugger); 
	

	//* ---------------------------------------- *
	// * Graph and asset testing stuff start here *
	// * ---------------------------------------- */


	// Create the plane with the collision shape
	btCollisionShape* groundShape = new btBoxShape(btVector3(1000.0f, 10.0f, 1000.0f));

	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),btVector3(0.0f, -10.0f, 0.0f)));

	btScalar groundMass = 0;
	btVector3 grounInertia(0,0,0);
	groundShape->calculateLocalInertia(groundMass,grounInertia);

	btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(groundMass, groundMotionState, groundShape, grounInertia);
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	dynamicsWorld->addRigidBody(groundRigidBody);

	Plane testPlaneGeom(2000.0f, 2000.0f);
	testPlane = new SceneNode(rootNodePtr, "test plane", &testPlaneGeom, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 1.0f, groundRigidBody);
	testPlane->getTransformation()->setBBTranslation(Vector(0, 10, 0));
	testPlane->setVisible(true);

	


	// Create character "rotationabout" node
	Sphere rotation_sphere = Sphere(50, 30, 30, true);
	SceneNode rotationCenter(rootNodePtr, "rotationCenter", &rotation_sphere, Vector(100.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
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

	
	md2Interface lostSoul_g = md2Interface(assetManagerPtr->getMd2Mesh("md2LostSoul"), assetManagerPtr->getTexture("lostSoulTx"));
	lostSoul = new SceneNode(&rotationCenter, "LostSoul", &lostSoul_g, Vector(200.0f, 100.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	lostSoul->scale(1, 1, 1);


	



	/*Sphere rotation_sphere2 = Sphere(10, 10, 10, true);
	SceneNode rotationCenter2(battleDroid, "rotationCenter2", &rotation_sphere, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	rotationCenter.setVisible(true);*/

	btCollisionShape* cubeShape = new btBoxShape(btVector3(10.0f, 10.0f, 10.0f));

	btDefaultMotionState* cubeMotionState;

	btScalar cubeMass = 0;
	btVector3 cubeInertia(0,0,0);
	cubeShape->calculateLocalInertia(cubeMass,cubeInertia);

	btRigidBody::btRigidBodyConstructionInfo * cubeRigidBodyCI;

	btRigidBody* cubeRigidBody;

	
	cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(0,-15,100)));
	cubeRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(cubeMass,cubeMotionState,cubeShape,cubeInertia);

	cubeRigidBody = new btRigidBody(*cubeRigidBodyCI);

	//cubeRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(cubeRigidBody);

	md2Interface * bossCube_g = new md2Interface(assetManagerPtr->getMd2Mesh("md2BossCube"), assetManagerPtr->getTexture("bossCubeTx"));
	bossCube = new SceneNode(battleDroid, "boss cube", bossCube_g, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, cubeRigidBody);



	
	cubeMass = 10;

	cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(100,300,-125)));
		
	cubeRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(cubeMass,cubeMotionState,cubeShape,cubeInertia);

	cubeRigidBody = new btRigidBody(*cubeRigidBodyCI);

	cubeRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(cubeRigidBody);

	bossCube_g = new md2Interface(assetManagerPtr->getMd2Mesh("md2BossCube"), assetManagerPtr->getTexture("bossCubeTx"));
	SceneNode * bossCube2 = new SceneNode(rootNodePtr, "falling boss cube", bossCube_g, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f, cubeRigidBody);


	/* ------------------------------------------ *
	 * Graph and asset testing stuff ends here  *
	 * ---------------------------------------- */

	// Set up camera and spectator
	entitySpectator *player = new entitySpectator();
	entityCamera *playercamera = new entityCamera();
	player->setCamera(playercamera);

	Controller::getInstance().setPlayerObject(player);

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


	while(!Controller::quit)
	{
		
		
		sprintf_s(title, "Name Here Engine | %i FPS", renderClock.getFPS() );
		window.setTitle( title, "include/nhe.ico" );
		
		rotationCenter.rotateAboutAxis(Vector(0,1,0),0.05f);
		rotationCenter.translate(Vector(0,0,0.1));
		//battleDroid->rotateAboutAxis(Vector(1,0,0),0.05f);
		bossCube->rotateAboutAxis(Vector(0,1,0),2.05f);
		bossCube2->rotateAboutAxis(Vector(0,0,1),0.02f);
		bossCube2->translate(Vector(0.1f,0,0));
		//rotationCenter2.rotateAboutAxis(Vector(0,1,0),-0.25f);


		

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
				Controller::quit = true;
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

		Controller::playerObject->update();
		
		// Actual frame rendering happens here
		if (window.getActive() )
		{
			//tickFrame = SDL_GetTicks();
			renderClock.frameUpdate();


			// enable light0
			glEnable(GL_LIGHT0);

			// sets ambient and diffuse components of light0
			glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
			glLightfv(GL_LIGHT0, GL_POSITION, Position);


			drawGL();
		}		
		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	//wait for the threads to exit
	SDL_WaitThread ( id1, NULL );
	SDL_WaitThread ( id2, NULL );
	SDL_WaitThread ( id3, NULL );

	exit(0);  
}

/* Draw the scene */
void drawGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glLoadIdentity();

	// Set the camera
	float *CamTransform = getCamera();

	if(drawDebug) dynamicsWorld->debugDrawWorld();	

	//Root::drawGeometry();
	AssetManager::lockMutex( rootNodePtr->mutex_node );
	rootNodePtr->drawGeometry();
	AssetManager::unlockMutex( rootNodePtr->mutex_node );

	// ********************
	// *** UPDATE POINT *** 
	// ********************

	// draw the animation
	AssetManager::lockMutex( rootNodePtr->mutex_node );
	rootNodePtr->update(0.06);
	AssetManager::unlockMutex( rootNodePtr->mutex_node );

	// Swaps the buffers
	SDL_GL_SwapBuffers();
}

/* Initialize OpenGL */
int initGL(void)
{
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
	//glEnable(GL_TEXTURE_2D);
	// enables smooth shading (garaud)
	glShadeModel(GL_SMOOTH);
	// frontfacing culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// enables lighting
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
		
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	//assetManagerPtr->loadMd2("include/Cyber.md2", "md2Demon");
	assetManagerPtr->loadMd2("include/battledroid.md2", "battleDroid");
	assetManagerPtr->loadMd2("include/Lostsoul.md2", "md2LostSoul");
	assetManagerPtr->loadMd2("include/bosscube.md2", "md2BossCube");
	assetManagerPtr->loadCollada("include/duck.dae", "duck");

	//assetManagerPtr->loadTexture("include/cyber.jpg", "doomDemonTx");
	assetManagerPtr->loadTexture("include/battledroid.png", "battleDroidTx");
	assetManagerPtr->loadTexture("include/lostsoul.jpg", "lostSoulTx");
	assetManagerPtr->loadTexture("include/bosscube.jpg", "bossCubeTx");
	assetManagerPtr->loadTexture("include/duckCM.tga", "duckCM.tga");

	//// ******************************
	//// ******** DEBUG INFO **********
	//// ******************************
	
	// write memory usage
	//std::cout << "memory usage demon " << (assetManagerPtr->getMd2Mesh("md2Demon")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage battle droid " << (assetManagerPtr->getMd2Mesh("battleDroid")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage lost soul " << (assetManagerPtr->getMd2Mesh("md2LostSoul")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage boss cube " << (assetManagerPtr->getMd2Mesh("md2BossCube")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage COLLADA duck " << (assetManagerPtr->getColladaMesh("duck")->getDataSize()/1024.0f) << "kb\n";
	return TRUE;
}

/* Apply camera matrices */
float* getCamera()
{		
	glMatrixMode(GL_MODELVIEW);

	entityCamera *currentCamera = Controller::playerObject->getCamera();

	float tranM[16];

	Matrix::generateIdentityMatrix().getMatrix(&tranM[0]);

	if ( currentCamera != NULL )
	{
		AssetManager::lockMutex( currentCamera->mutex_object );

		Matrix transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(1.0,0.0,0.0),currentCamera->pitch)).getTranspose();
		transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(0.0,1.0,0.0),currentCamera->yaw)).getTranspose() * transformationMatrix;
		transformationMatrix = Matrix::generateTranslationMatrix(	currentCamera->vPosition[0],
																	currentCamera->vPosition[1],
																	currentCamera->vPosition[2]).getTranspose() * transformationMatrix;
		transformationMatrix.getMatrix(&tranM[0]);

		AssetManager::unlockMutex( currentCamera->mutex_object );
	}

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}
