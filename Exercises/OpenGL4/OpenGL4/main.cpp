#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>					// Header File for SDL thread library
#include <SDL_audio.h>					// Header File for SDL audio library
#include <glut.h>						// Header File for glut commands

#include "linearAlgebraDLL.h"			// Header File for our math library
#include "sceneNode.h"					// Header File for the SceneNode/Scenegraph
#include "sceneObject.h"				// Header File for the SceneObject container
#include "md2File.h"					// Header File for our md2 loader
#include "colladaFile.h"				// Header File for out COLLADA loader
#include "assetManager.h"				// Header File for our Asset Manager
#include "inputManager.h"				// Header File for our Input Manager

#define NUM_SOUNDS 2

// Type for Sound data
struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

using namespace std;
using namespace linearAlgebraDLL;

static int const screenWidth		= 800;			// Window Width
static int const screenHeight		= 600;			// Window Height
static int const screenColorDepth	= 32;			// Color Depth
static int const tick				= 16;			// Minimum time between screen frames
static int const thread_delay		= 3;			// Minimum time between loops
static float const PI = 3.14159f;					// PI definition

Uint32 tickFrame = 0;

// Message pump used for passing Events between threads
MessagePump InputPump;								

// Pointer to SDL rendering surface
SDL_Surface * surface;				

GLuint image;	

// Root node and other Scene Node
Root *rootNodePtr;
// Asset manager
AssetManager * assetManagerPtr;

SceneNode * demon;
//md2File md2Demon;
SceneNode * lostSoul;
//md2File md2LostSoul;
SceneNode * bossCube;
//md2File md2BossCube;
SceneNode * colladaDuck;
//ColladaFile colladaTest;

// Camera and Movements Definitions
float camYaw, camPitch, camYawRad, camPitchRad;
float camPosX, camPosY, camPosZ;
float camSpeed = 0.5f;
float rotationSpeed = 0.1f;

// OpenGL Attributes
Uint32 timeLeft(void);
GLuint loadTexture(char* texName);
int resizeWindow(int width, int height);
int initGL(void);

// STD/OpenGL Methods
void drawGL(void);

// Pointer to the function that moves the camera
float* getCamera();

GLuint	filter;
GLuint	texture[3];

// input manager definition
inputManager input;

// Define Test Lights Attributes
GLfloat Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};  
GLfloat Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 

// Check for when to quit
bool quit = false;

/* This thread handles input */
int threadInput(void *data)
{
	input = input.getInstance();
	char *tname = ( char * )data;

	// Disable the Windows Cursor
	SDL_ShowCursor(SDL_DISABLE); 
	
	// Binds mouse and keyboard input to the OpenGL window
	SDL_WM_GrabInput(SDL_GRAB_ON); 

	while ( !Controller::getInstance().quit ) {
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
	// Thread name
	char *tname = ( char * )data;

	extern void mixaudio(void *unused, Uint8 *stream, int len);
    SDL_AudioSpec fmt;

    /* Set 16-bit stereo audio at 22Khz */
    fmt.freq = 22050;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;
    fmt.callback = mixaudio;
    fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		return 1;
    }
    SDL_PauseAudio(0);

	// Run the audio handling here

	SDL_CloseAudio();
	return 0;
}

/* This thread updates the scene */
int threadUpdate(void *data)
{
	// Thread name
	char *tname = ( char * )data;

	while ( !Controller::getInstance().quit ) {
		// Runs the update method here

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	return 0;
}

/* Application entry point */
int main(int argc, char *argv[])
{
	// start the asset manager
	assetManagerPtr = new AssetManager();
	InputPump = MessagePump();

	// SDL/OpenGL data
	int videoFlags;
	const SDL_VideoInfo *videoInfo;
	
	int isActive = TRUE;
	
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		exit(1);
	}

	videoInfo = SDL_GetVideoInfo();
	if (!videoInfo)
	{
		exit(1);
	}
	
	// Video Flags
	videoFlags = SDL_OPENGL;
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	videoFlags |= SDL_RESIZABLE;
	
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;
	
	SDL_WM_SetCaption( "Loading Name Here Engine... ", "include/nhe.ico" );

	// Apply Video Flags and Settings
	surface = SDL_SetVideoMode(	screenWidth,
								screenHeight,
								screenColorDepth,
								videoFlags);

	if (!surface)
	{
		
		exit(1);
	}

	if (initGL()==FALSE)
	{
		
		exit(1);
	}
	
	// resizes OpenGL window
	resizeWindow(screenWidth, screenHeight);
	
	// Create the root node
	rootNodePtr = new Root();
	rootNodePtr->setName("root");

	// Define the exit of the program in relation to SDL variables
	atexit(SDL_Quit);

	// Create the thread handles and assign names
	SDL_Thread *id1;
	SDL_Thread *id2;
	SDL_Thread *id3;
	char *tnames[] = { "General Update", "Sound Manager", "Input"};

	// Create the threads
	id1 = SDL_CreateThread ( threadUpdate, tnames[0] );
	id2 = SDL_CreateThread ( threadSound, tnames[1] );
	id3 = SDL_CreateThread ( threadInput, tnames[2] );

	/* ---------------------------------------- *
	 * Graph and asset testing stuff start here *
	 * ---------------------------------------- */

	assetManagerPtr->loadTexture("include/cyber.jpg", "doomDemonTx");
	assetManagerPtr->loadTexture("include/lostsoul.jpg", "lostSoulTx");
	assetManagerPtr->loadTexture("include/bosscube.jpg", "bossCubeTx");

	rootNodePtr->lock(); // Node needs to be locked because we're adding a child to it in the next two lines
	
	md2Interface doomDemon = md2Interface(assetManagerPtr->getMd2Mesh("md2Demon"), assetManagerPtr->getTexture("doomDemonTx"));
	demon = new SceneNode(rootNodePtr, "Doom Demon", &doomDemon, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	rootNodePtr->unlock(); // We can unlock the node now

	demon->lock(); // The new node needs to be locked now since we're doing a transform on it, then adding a child
	demon->scale(0.8, 0.8, 0.8);
	
	Sphere kernel_sphere = Sphere(50, 30, 30, true);
	SceneNode kernel(demon, "kernel", &kernel_sphere, Vector(0.0f, 100.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	kernel.setVisible(0);
	demon->unlock(); // We can now unlock it
	
	kernel.lock();
	md2Interface lostSoul_g = md2Interface(assetManagerPtr->getMd2Mesh("md2LostSoul"), assetManagerPtr->getTexture("lostSoulTx"));
	lostSoul = new SceneNode(&kernel, "LostSoul", &lostSoul_g, Vector(200.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	kernel.unlock();

	// why here? - ask Simon
	lostSoul->lock();
	lostSoul->scale(1, 1, 1);
	lostSoul->unlock();
	
	md2Interface bossCube_g = md2Interface(assetManagerPtr->getMd2Mesh("md2BossCube"), assetManagerPtr->getTexture("bossCubeTx"));
	bossCube = new SceneNode(lostSoul, "boss cube", &bossCube_g, Vector(100.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	lostSoul->unlock();

	bossCube->lock();
	bossCube->scale(0.8, 0.8, 0.8);
	bossCube->unlock();

	ColladaInterface fuckingDuck_g = ColladaInterface(assetManagerPtr->getColladaMesh("fuckingDuck"));
	colladaDuck = new SceneNode(rootNodePtr, "fuckingDuck", &fuckingDuck_g,  Vector(0.0f, 10.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);

	/* ---------------------------------------- *
	 * Graph and asset testing stuff ends here  *
	 * ---------------------------------------- */

	// Set up camera and spectator
	entitySpectator *player = new entitySpectator();
	entityCamera *playercamera = new entityCamera();
	player->setCamera(playercamera);

	Controller::getInstance().setPlayerObject(player);

	while(!Controller::getInstance().quit)
	{
		
		char title[80];
		sprintf_s(title, "Name Here Engine");
		SDL_WM_SetCaption( title, NULL );
		
		kernel.rotateAboutAxis(Vector(0,1,0),0.2f);
		//lostSoul->rotateAboutAxis(Vector(0,1,0),0.3f);
		//bossCube->rotateAboutAxis(Vector(1,0,1),0.4f);
		
		// Time to take care of the SDL events we have recieved
		SDL_Event currentEvent;
		while(SDL_PollEvent(&currentEvent))
		{
			switch(currentEvent.type)
			{
			// Some general events to handle immediately
			case SDL_ACTIVEEVENT:
				if (currentEvent.active.gain==0)
					isActive = FALSE;
				else
					isActive = TRUE;
				break;
			case SDL_VIDEORESIZE:
				surface = SDL_SetVideoMode(	currentEvent.resize.w, currentEvent.resize.h, screenColorDepth, videoFlags);
				if (!surface)
				{
					delete assetManagerPtr;
					exit(1);
				}
				resizeWindow( currentEvent.resize.w, currentEvent.resize.h);
				break;
			case SDL_QUIT:
				Controller::getInstance().quit = true;
				break;
			// Input events coming below. They are all just passed on to the input pump
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				InputPump.lock();
				InputPump.sendMessage(currentEvent);
				InputPump.unlock();
				break;
			// Anything else we don't care about
			default:
				break;
			}
		}

		Controller::getInstance().playerObject->update();
		
		// Actual frame rendering happens here
		if (isActive && SDL_GetTicks() > (tickFrame + tick) )
		{
			tickFrame = SDL_GetTicks();
			drawGL();

		}
	
		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	//wait for the threads to exit
	SDL_WaitThread ( id1, NULL );
	SDL_WaitThread ( id2, NULL );
	SDL_WaitThread ( id3, NULL );

	// Deletes
	//delete demon;
	//delete lostSoul; 
	//delete bossCube;
	//delete playercamera;
	//delete player;
	//delete rootNodePtr;
	//delete assetManagerPtr;
	
	exit(0);  
}

/* Draw the scene */
void drawGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glLoadIdentity();

	// Set the camera
	float *CamTransform = getCamera();

	//Root::drawGeometry();
	rootNodePtr->lock();
	rootNodePtr->drawGeometry();
	rootNodePtr->unlock();
	//glPopMatrix();

	// ********************
	// *** UPDATE POINT *** 
	// ********************

	// draw the animation
	rootNodePtr->lock();
	rootNodePtr->update(0.06);
	rootNodePtr->unlock();

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
	glViewport(0,0,screenWidth,screenHeight);
	glLoadIdentity();
	// sets the matrix stack as the modelview matrix stack
	glMatrixMode(GL_MODELVIEW);
	
	// enables the Z-buffer
	glEnable(GL_DEPTH_TEST);
	// enables the texture rendering
	glEnable(GL_TEXTURE_2D);
	// enables smooth shading (garaud)
	glShadeModel(GL_SMOOTH);
	// frontfacing culling
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);*/
	// enables lighting
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	// enable light0
	glEnable(GL_LIGHT0);

	// sets ambient and diffuse components of light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
		
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	assetManagerPtr->loadMd2("include/Cyber.md2", "md2Demon");
	assetManagerPtr->loadMd2("include/Lostsoul.md2", "md2LostSoul");
	assetManagerPtr->loadMd2("include/bosscube.md2", "md2BossCube");
	assetManagerPtr->loadCollada("include/duck.dae", "fuckingDuck");

	assetManagerPtr->loadTexture("include/cyber.jpg", "doomDemonTx");
	assetManagerPtr->loadTexture("include/lostsoul.jpg", "lostSoulTx");
	assetManagerPtr->loadTexture("include/bosscube.jpg", "bossCubeTx");

	//colladaTest.load("include/duck.dae");

	//// ******************************
	//// ******** DEBUG INFO **********
	//// ******************************
	
	// write memory usage
	std::cout << "memory usage demon " << (assetManagerPtr->getMd2Mesh("md2Demon")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage lost soul " << (assetManagerPtr->getMd2Mesh("md2LostSoul")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage boss cube " << (assetManagerPtr->getMd2Mesh("md2BossCube")->GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage COLLADA duck " << (assetManagerPtr->getColladaMesh("fuckingDuck")->getDataSize()/1024.0f) << "kb\n";
	return TRUE;
}

/* Apply camera matrices */
float* getCamera()
{		
	glMatrixMode(GL_MODELVIEW);

	entityCamera *currentCamera = Controller::getInstance().playerObject->getCamera();

	float tranM[16] = {	1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 0};

	if ( currentCamera != NULL )
	{
		currentCamera->lock();

		Matrix transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),currentCamera->pitch).getTranspose();
		transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),currentCamera->yaw).getTranspose() * transformationMatrix;
		transformationMatrix = Matrix::generateTranslationMatrix(	currentCamera->vPosition[0],
																	currentCamera->vPosition[1],
																	currentCamera->vPosition[2]).getTranspose() * transformationMatrix;
		transformationMatrix.getMatrix(&tranM[0]);

		currentCamera->unlock();
	}

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}

/* Resize the window */
int resizeWindow(int width, int height)
{
	GLfloat ratio;
	if (height==0)
		height = 1;

	// define the new pixel aspect
	ratio = (GLfloat)width/(GLfloat)height;
	glViewport(0,0,(GLint)width,(GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// sets the Field of View, pixel ratio, Frustum
	gluPerspective(60.0f, ratio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return TRUE;
}

// Audio mixer for SDL sounds
void mixaudio(void *unused, Uint8 *stream, int len)
{
    int amount;

    for (int i=0; i<NUM_SOUNDS; ++i ) {
        amount = (sounds[i].dlen-sounds[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
        SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
        sounds[i].dpos += amount;
    }
}


// Function that load a sound, converti it and let it start to play
void PlaySound(char *file)
{
    int index;
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    /* Look for an empty (or finished) sound slot */
    for ( index=0; index<NUM_SOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }
    if ( index == NUM_SOUNDS )
        return;

    /* Load the sound file and convert it to 16-bit stereo at 22kHz */
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
                            AUDIO_S16,   2,             22050);
    cvt.buf = (Uint8 *)malloc(dlen*cvt.len_mult);
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(data);

    /* Put the sound data in the slot (it starts playing immediately) */
    if ( sounds[index].data ) {
        free(sounds[index].data);
    }
    SDL_LockAudio();
    sounds[index].data = cvt.buf;
    sounds[index].dlen = cvt.len_cvt;
    sounds[index].dpos = 0;
    SDL_UnlockAudio();
}
