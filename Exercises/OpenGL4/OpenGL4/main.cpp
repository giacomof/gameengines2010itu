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
#include "messagePump.h"				// Header File for the input messahe pump system
#include "md2Loader.h"					// Header File for our md2 loader
#include "assetManager.h"

#define NUM_SOUNDS 2
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
MessagePump inputPump;								

// Pointer to SDL rendering surface
SDL_Surface *surface;				

GLuint image;	

// Root node and other Scene Node
Root * rootNodePtr;

AssetManager * assetManagerPtr;

SceneNode * demon;
md2Loader md2Demon;
SceneNode * lostSoul;
md2Loader md2LostSoul;
SceneNode * bossCube;
md2Loader md2BossCube;


// Define Lights Attributes
GLfloat Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};  
GLfloat Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 

// Camera and Movements Definitions
float camYaw, camPitch, camYawRad, camPitchRad;
float camPosX, camPosY, camPosZ;
float camSpeed = 0.5f;
float rotationSpeed = 0.1f;

// Camera input states
int mouseStateX, mouseStateY, centerX=0, centerY=0, dX, dY, temp;
int wKeyPressed = 0, sKeyPressed = 0, aKeyPressed = 0, dKeyPressed = 0, lKeyPressed = 0;

// OpenGL Attributes
Uint32 timeLeft(void);
GLuint loadTexture(char* texName);
int resizeWindow(int width, int height);
int initGL(void);

// STD/OpenGL Methods
void drawGL(void);
void keyDown(SDL_keysym *keysym);
void keyUp(SDL_keysym *keysym);
void update();

// Pointer to the function that moves the camera
float* getCamera();
// Function that restricts camera movements
void clampCamera();

GLuint	filter;
GLuint	texture[3];

// Defines when to stop looping
bool quit = false;

/* This thread handles audio */
int soundmngr(void *data)
{
	// Thread name
	char *tname = ( char * )data;

	extern void mixaudio(void *unused, Uint8 *stream, int len);
    SDL_AudioSpec fmt;

    /* Set 16-bit stereo audio at 22Khz */
    fmt.freq = 22050;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;        /* A good value for games */
    fmt.callback = mixaudio;
    fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		return 1;
		// exit(1);
    }
    SDL_PauseAudio(0);

	SDL_CloseAudio();
	return 0;
}

/* This thread updates the scene */
int updater(void *data)
{
	// Thread name
	char *tname = ( char * )data;

	while ( !quit ) {
		// Runs the update scene method
		update();
		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	exit(0);
}

/* Application entry point */
int main(int argc, char *argv[])
{

	// SDL/OpenGL data
	int videoFlags;
	SDL_Event event;
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
	
	//SDL_WM_SetCaption( "Loading Name Here Engine... ", "include/nhe.ico" );

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
	
	// Disable the Windows Cursor
	ShowCursor(SDL_DISABLE); 
	
	// Binds mouse and keyboard input to the OpenGL window
	SDL_WM_GrabInput(SDL_GRAB_ON); 
	
	

	// Move the mouse in the center of the windows before starting render
	SDL_WarpMouse((short)centerX, (short)centerY);

	// Create the root node
	rootNodePtr = new Root();

	assetManagerPtr = new AssetManager();

	// The updater thread
	SDL_Thread *id1;
	SDL_Thread *id2;
	char *tnames[] = { "Updater", "Sound Manager" };

	// Define the exit of the program in relation to SDL variables
	atexit(SDL_Quit);

	//create the threads
	id1 = SDL_CreateThread ( updater, tnames[0] );
	id2 = SDL_CreateThread ( soundmngr, tnames[1] );

	/*Geometry bigTriangle = Geometry(0);
	bigTriangle.addVertex(&Point(0.0f, 0.0f, 0.0f));
	bigTriangle.addVertex(&Point(100.0f, 0.0f, 0.0f));
	bigTriangle.addVertex(&Point(0.0f, 0.0f, -100.0f));

	SceneNode plane(rootNodePtr, "Triangle Plane", &bigTriangle, 0.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	SceneNode plane2(&plane, "Triangle Plane2", &bigTriangle, 100.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	plane2.scale(1,1,1);
	SceneNode plane3(&plane2, "Triangle Plane3", &bigTriangle, 50.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	plane3.scale(1,1,1);*/

	assetManagerPtr->loadTexture("include/cyber.jpg", "doomDemonTx");
	assetManagerPtr->loadTexture("include/lostsoul.jpg", "lostSoulTx");
	assetManagerPtr->loadTexture("include/bosscube.jpg", "bossCubeTx");

	rootNodePtr->lock(); // Node needs to be locked because we're adding a child to it in the next two lines
	
	md2File doomDemon = md2File(&md2Demon, assetManagerPtr->getTexture("doomDemonTx"));
	demon = new SceneNode(rootNodePtr, "Doom Demon", &doomDemon, Vector(0.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	rootNodePtr->unlock(); // We can unlock the node now

	demon->lock(); // The new node needs to be locked now since we're doing a transform on it, then adding a child
	demon->scale(0.8, 0.8, 0.8);
	
	Sphere kernel_sphere = Sphere(50, 30, 30, true);
	SceneNode kernel(demon, "kernel", &kernel_sphere, Vector(0.0f, 100.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	kernel.setVisible(0);
	demon->unlock(); // We can now unlock it
	
	kernel.lock();
	md2File lostSoul_g = md2File(&md2LostSoul, assetManagerPtr->getTexture("lostSoulTx"));
	lostSoul = new SceneNode(&kernel, "LostSoul", &lostSoul_g, Vector(200.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	kernel.unlock();


	// why here? - ask Simon
	lostSoul->lock();
	lostSoul->scale(1, 1, 1);
	lostSoul->unlock();
	
	md2File bossCube_g = md2File(&md2BossCube, assetManagerPtr->getTexture("bossCubeTx"));
	bossCube = new SceneNode(lostSoul, "boss cube", &bossCube_g, Vector(100.0f, 0.0f, 0.0f), Vector(0.0f,0.0f,0.0f), 0.0f);
	lostSoul->unlock();

	bossCube->lock();
	bossCube->scale(0.8, 0.8, 0.8);
	bossCube->unlock();

	while(!quit)
	{
		
		char title[80];
		sprintf_s(title, "Name Here Engine");
		SDL_WM_SetCaption( title, NULL );
		
		kernel.rotateAboutAxis(Vector(0,1,0),0.2f);
		//lostSoul->rotateAboutAxis(Vector(0,1,0),0.3f);
		bossCube->rotateAboutAxis(Vector(1,0,1),0.4f);
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_ACTIVEEVENT:
				if (event.active.gain==0)
					isActive = FALSE;
				else
					isActive = TRUE;
				break;
			case SDL_VIDEORESIZE:
				surface = SDL_SetVideoMode(	event.resize.w,
											event.resize.h,
											screenColorDepth,
											videoFlags);
				if (!surface)
				{
					exit(1);
				}

				resizeWindow( event.resize.w, event.resize.h);
				break;
			case SDL_KEYDOWN:
				keyDown(&event.key.keysym);
				break;
			case SDL_KEYUP:
				keyUp(&event.key.keysym);
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouseStateX, &mouseStateY);
				dX = (int)mouseStateX - centerX;
				dY = (int)mouseStateY - centerY;
				camYaw -= rotationSpeed * dX;
				camPitch -= rotationSpeed * dY;
				clampCamera();
				SDL_WarpMouse((short)centerX, (short)centerY);
				break;

			case SDL_QUIT:
				quit = TRUE;
				break;
			default:
				break;
			}
		}
		
		if (isActive && SDL_GetTicks() > (tickFrame + tick) )
		//if(true)
		{
			tickFrame = SDL_GetTicks();
			drawGL();

		}
	
		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}
	ShowCursor(TRUE);

	//wait for the threads to exit
	SDL_WaitThread ( id1, NULL );
	SDL_WaitThread ( id2, NULL );

	// Delete the message pump between threads
	delete &inputPump;
	
	exit(0);  
}

/* Draw the scene */
void drawGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glLoadIdentity();

	// Set the camera
	float *CamTransform = getCamera();

	// Binds the "image" texture to the OpenGL object GL_TEXTURE_2D
	//glBindTexture(GL_TEXTURE_2D, md2Texture);

	//Root::drawGeometry();
	rootNodePtr->lock();
	rootNodePtr->drawGeometry();
	rootNodePtr->unlock();
	//glPopMatrix();

	// ********************
	// *** UPDATE POINT *** 
	// ********************

	// draw the animation
	demon->lock();
	demon->update(0.006);
	demon->unlock();

	lostSoul->lock();
	lostSoul->update(0.006);
	lostSoul->unlock();
	
	bossCube->lock();
	bossCube->update(0.006);
	bossCube->unlock();
	


	// Swaps the buffers
	SDL_GL_SwapBuffers();
}


/* Update gamestate */
void update()
{
	//lock variables
	//SDL_mutexP ( value_mutex );

	// The initial orientation, to be modified by pitch and yaw
	Vector CamForward	(0.0, 0.0, -1.0	);
	Vector CamSideways	(1.0, 0.0, 0.0	);
	Vector CamUp		(0.0, 1.0, 0.0	);

	// If we need to move, find the actual forward and sideway vectors
	if ( (wKeyPressed+sKeyPressed+aKeyPressed+dKeyPressed) > 0 ) {
		Matrix CamMatrix;
		CamMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),-camPitch).getTranspose();
		CamMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),-camYaw).getTranspose() * CamMatrix;
		
		CamForward = CamMatrix * CamForward;
		CamSideways = CamMatrix * CamSideways;
		CamUp = CamMatrix * CamUp;
		
	}

	// Forward
	if (wKeyPressed && !sKeyPressed) {
		camPosX -= CamForward[0] * camSpeed;
		camPosY -= CamForward[1] * camSpeed;
		camPosZ -= CamForward[2] * camSpeed;
	}
	
	// Backwards
	if (sKeyPressed && !wKeyPressed) {
		camPosX += CamForward[0] * camSpeed;
		camPosY += CamForward[1] * camSpeed;
		camPosZ += CamForward[2] * camSpeed;
	}
	
	// Left
	if (aKeyPressed && !dKeyPressed) {
		camPosX += CamSideways[0] * camSpeed;
		camPosY += CamSideways[1] * camSpeed;
		camPosZ += CamSideways[2] * camSpeed;
	}
	
	// Right
	if (dKeyPressed && !aKeyPressed) {
		camPosX -= CamSideways[0] * camSpeed;
		camPosY -= CamSideways[1] * camSpeed;
		camPosZ -= CamSideways[2] * camSpeed;
	}
	if (lKeyPressed) {
		PlaySound("include/evil_laugh.wav", NULL, SND_ALIAS | SND_APPLICATION);
	}

	//release the lock 
	//SDL_mutexV ( value_mutex );
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
	
	// enables lighting
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	// enable light0
	glEnable(GL_LIGHT0);

	// sets ambient and diffuse components of light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
	
	
	// defines the center of the screen
	centerX = screenWidth/2;
	centerY = screenHeight/2;
	
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	md2Demon = md2Loader();
	md2LostSoul = md2Loader();
	md2BossCube = md2Loader();

	//// loads md2 files
	md2Demon.Load("include/Cyber.md2");
	md2LostSoul.Load("include/Lostsoul.md2");
	md2BossCube.Load("include/bosscube.md2");

	//// ******************************
	//// ******** DEBUG INFO **********
	//// ******************************
	
	// write memory usage
	std::cout << "memory usage demon " << (md2Demon.GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage lost soul " << (md2LostSoul.GetDataSize()/1024.0f) << "kb\n";
	std::cout << "memory usage boss cube" << (md2BossCube.GetDataSize()/1024.0f) << "kb\n";
	//glColor3f(1,1,1);

	return TRUE;
}

/* Check key press events */
void keyDown(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
	case SDLK_ESCAPE:
		exit(0);
		break;
	case SDLK_F1:
		SDL_WM_ToggleFullScreen(surface);
		break;
	case SDLK_w:
		wKeyPressed = 1;
		break;
	case SDLK_s:
		sKeyPressed = 1;
		break;
	case SDLK_a:
		aKeyPressed = 1;
		break;
	case SDLK_d:
		dKeyPressed = 1;
		break;
	case SDLK_l:
		lKeyPressed = 1;
		break;
	case SDLK_0:
		md2Demon.SetAnim(0);
		break;
	case SDLK_1:
		md2Demon.SetAnim(1);
		break;
	case SDLK_2:
		md2Demon.SetAnim(2);
		break;
	case SDLK_3:
		md2Demon.SetAnim(3);
		break;
	case SDLK_4:
		md2Demon.SetAnim(4);
		break;
	case SDLK_5:
		md2Demon.SetAnim(5);
		break;
	case SDLK_6:
		md2Demon.SetAnim(6);
		break;
	case SDLK_7:
		md2Demon.SetAnim(7);
		break;
	case SDLK_8:
		md2Demon.SetAnim(8);
		break;
	case SDLK_9:
		md2Demon.SetAnim(9);
		break;
	default:
		break;
	}
	return;
}

/* Check key release events */ 
void keyUp(SDL_keysym *keysym) 
{
	switch (keysym->sym)
    {
		case SDLK_w:
			wKeyPressed = 0;
			break;
		case SDLK_s:
			sKeyPressed = 0;
			break;
		case SDLK_a:
			aKeyPressed = 0;
			break;
		case SDLK_d:
			dKeyPressed = 0;
			break;
		case SDLK_l:
			lKeyPressed = 0;
			break;
		default:
			break;
	}
}

/* Apply camera matrices */
float* getCamera()
{		
	glMatrixMode(GL_MODELVIEW);

	float tranM[16];
	Matrix transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),-camPitch).getTranspose();
	transformationMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),-camYaw).getTranspose() * transformationMatrix;
	transformationMatrix = Matrix::generateTranslationMatrix(camPosX, camPosY, camPosZ).getTranspose() * transformationMatrix;
	transformationMatrix.getMatrix(&tranM[0]);

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}

/* Clamps the camera to a specific pitch and constrains yaw */
void clampCamera()
{
	// Limits camera pitch
	if (camPitch>90.0f)
		camPitch = 90.0f;
	else if (camPitch<-90.0f)
		camPitch = -90.0f;

	// Rolls over yaw
	while(camYaw<=0.0f)
		camYaw += 360.0f;
	while(camYaw>=360.0f)
		camYaw -= 360.0f;
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
