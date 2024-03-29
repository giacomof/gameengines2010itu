#ifdef __DDEngine
#    define DDEngine_D __declspec(dllexport)
#else
#    define DDEngine_D __declspec(dllimport)
#endif

#ifndef DDEngine__H 
#define DDEngine__H

#include "assetManager.h"						// Header File for our Asset Manager
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

#include "mutexManager.h"						// Header File for our Mutex Manager
#include "memoryManager.h"						// Header File for our Memory Manager
#include "linearAlgebra.h"						// Header File for our math library
#include "sceneNode.h"							// Header File for the SceneNode/Scenegraph
#include "sceneObject.h"						// Header File for the SceneObject container
#include "md2File.h"							// Header File for our md2 loader
#include "colladaFile.h"						// Header File for out COLLADA loader
#include "inputManager.h"						// Header File for our Input Manager
#include "frameClock.h"							// Header File for our Clock
#include "sound.h"								// Header File for our Sound
#include "windowManager.h"						// Header File for our Window Manager
#include "debugDraw.h"							// Header File for our Debug Drawer 
#include "log.h"								// Header File for our Log Manager
#include "scriptHandler.h"						// Header File for our Script Handler

using namespace std;


class DDEngine_D DDEngine
{
public:

	static SDL_mutex * mutex_allocator;

	DDEngine(int screenWidth, int screenHeight, int colorDepth, bool physic);
	~DDEngine(void);
	
	void run();
	void printDebugInfo();
	
	virtual void setupScene() = 0;
	virtual void frameStarted(int frameDelta) = 0;
	virtual void frameEnded(int frameDelta) = 0;
	
	inputManager * getInputManager() { return &input; };
	Controller * getController() { return &controller; };
	btDiscreteDynamicsWorld * getDynamicWorld() { return dynamicsWorld; };
	
	btRigidBody * createPhysicalBox(Vector dimension, Vector position, Quaternion orientation, float mass, bool neverSleep = true);
	btRigidBody * createPhysicalSphere(float radius, Vector position, Quaternion orientation, float mass, bool neverSleep = true);
	btRigidBody * createRigidBody(btCollisionShape * collisionShape, Vector position, Quaternion orientation, float mass, bool neverSleep = true);
	btCollisionShape * createCollisionBox(Vector dimension);
	btCollisionShape * createCollisionSphere(float radius);

	SceneObject * createMD2(md2File * model, unsigned int texture = 0, int shaderFlag = 0);
	SceneObject * createCollada(ColladaFile * model, unsigned int texture = 0, int shaderFlag = 0, ColladaSkeleton * skeleton = 0);
	SceneObject * createSphere(float radius, int slices, int stacks, bool wireframe, int shaderFlag = 0, unsigned int texture = 0);
	SceneObject * createPlane(float width, float height, int sideSubdivisions, int shaderFlag = 0, unsigned int texture = 0);
	SceneObject * createCube(float side, int shaderFlag = 0, unsigned int texture = 0);
	SceneObject * createLine(Vector start, Vector end, int shaderFlag = 0, unsigned int texture = 0);
	SceneObject * createTeapot(float size, bool wireframe, int shaderFlag = 0, unsigned int texture = 0);
	SceneObject * createLight(	bool enabled, bool directional,
								float ambientR, float ambientG, float ambientB,
								float diffuseR, float diffuseG, float diffuseB,
								float specularR, float specularG, float specularB);

	SceneNode * addSceneNode(SceneNode * father, char * name, SceneObject * geometry, Vector position, Vector quaternionVector, float quaternionRotation, btRigidBody * physicGeometry = 0);

	void runJSScript(const char * filePath);

	// FPS Clock
	frameClock renderClock;

protected:
	
	int screenW;								// Window Width
	int screenH;								// Window Height
	int screenCD;								// Color Depth
	bool hasPhysics;
	Vector physicGravity;

	// Message pumps used for passing Events between threads
	MessagePump InputPump;
	// Pointer to SDL rendering surface
	SDL_Surface * surface;				
	// Root node and other Scene Node
	Root rootNode;
	// Asset manager
	AssetManager assetManager;
	// Debug Drawing system
	DebugDraw debugger;
	// Input manager and Controller
	inputManager input;
	Controller controller;
	// Window Manager definition
	WindowManager window;
	// Entity spectator 
	entitySpectator * player;
	// Entity camera
	entityCamera * playercamera;
	// Skybox
	SkyBox * skyBox;

	// Physic world
	btDiscreteDynamicsWorld * dynamicsWorld;
	// Broadphase
	btDbvtBroadphase * broadphase;
	// Collision Configuration
    btDefaultCollisionConfiguration * collisionConfiguration;
	// Collision Dispatcher
    btCollisionDispatcher * dispatcher;
	// Constraint Solver
    btSequentialImpulseConstraintSolver * solver;

	// Script Handler
	ScriptHandler scriptHandler;
	
	int initGL(void);											// Initialise the rendering window
	int initPhysics(void);										// Initialise the physic engine
	void drawGL(int frameDelta);								// Draw the world
	float * getCamera(void);									// Move the camera
	void addSkyBox(float halfSide, unsigned int * textureList);	// Add a skybox to the engine

};

#endif;