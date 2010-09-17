#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>

#include "bmp.h"						// Header File for the glaux replacement library
#include "linearAlgebraDLL.h"			// Header File for our math library
#include "SceneNode.h"					// Header File for the SceneNode/Scenegraph
#include "messagePump.h"				// Header File for the input messahe pump system

using namespace std;
using namespace linearAlgebraDLL;

static int const screenWidth		= 800;			// Window Width
static int const screenHeight		= 600;			// Window Height
static int const screenColorDepth	= 32;			// Color Depth
static int const tick				= 16;			// Minimum time between screen frames
static int const thread_delay		= 3;			// Minimum time between loops

static float const PI = 3.14159f;

MessagePump inputPump;

SDL_Surface *surface;					
GLuint image;	
Root * rootNodePtr;


// Define Lights Attributes
// *************************************
// ********** GREEN LIGHTS *************
// *************************************
GLfloat Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};  
GLfloat Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 

// Camera and Movements Definitions
float camYaw, camPitch, camYawRad, camPitchRad;
float camPosX, camPosY, camPosZ;
float camSpeed = 0.1f;
float rotationSpeed = 0.1f;

// Camera input states
int mouseStateX, mouseStateY, centerX=0, centerY=0, dX, dY, temp;
int wKeyPressed = 0, sKeyPressed = 0, aKeyPressed = 0, dKeyPressed = 0;

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

float* getCamera();
void clampCamera();

GLuint	filter;
GLuint	texture[3];

//mutex to lock variables
SDL_mutex *value_mutex;

bool quit = false;


/* Thread for rendering the scene */
int openGlRenderer (void *data)
{
	char *tname = ( char * )data;
	int videoFlags;
	int done = FALSE;
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
	
	Uint32 tickFrame = 0;

	SDL_WarpMouse((short)centerX, (short)centerY);

	rootNodePtr = new Root();

	SceneNode plane(rootNodePtr, "Triangle Plane", 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f);
	plane.scale(0.5f, 1.0f, 2.0f);
	//plane.rotate(0.0f, 45.0f, 0.0f);


	while(!done)
	{
		//lock 
		//SDL_mutexP ( value_mutex ); 
		
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
				done = TRUE;
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

		//release the lock 
		//SDL_mutexV ( value_mutex );
		
		SDL_Delay(thread_delay);
	}
	ShowCursor(TRUE);
	
	exit(0);
}

/* This thread updates the scene */
int updater(void *data)
{
	char *tname = ( char * )data;

	while ( !quit ) {
		update();
		SDL_Delay(thread_delay);
	}

	exit(0);
}

/* Application entry point */
int main(int argc, char *argv[])
{
	// The main threads
	SDL_Thread *id1, *id2;
	char *tnames[2] = { "Renderer", "Updater" };

	atexit(SDL_Quit);

	// Create the mutex
	value_mutex = SDL_CreateMutex();

	//create the threads
	id1 = SDL_CreateThread ( openGlRenderer, tnames[0] );
	id2 = SDL_CreateThread ( updater, tnames[1] );

	//wait for the threads to exit
	SDL_WaitThread ( id1, NULL );
	SDL_WaitThread ( id2, NULL );

	// Release the mutex
	SDL_DestroyMutex ( value_mutex );

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
	
	//Root::drawGeometry();
	rootNodePtr->drawGeometry();

	// Binds the "image" texture to the OpenGL object GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, image);
	glLoadIdentity();
	
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
		
		// Prints to debug camera movement
		/*
		printf("\n");
		printf("Forward : [%f, %f, %f]\n",CamForward[0],CamForward[1],CamForward[2]);
		printf("Sideways: [%f, %f, %f]\n",CamSideways[0],CamSideways[1],CamSideways[2]);
		printf("Up      : [%f, %f, %f]\n",CamUp[0],CamUp[1],CamUp[2]);
		*/
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
	default:
		break;
	}
}

/* Apply camera matrices */
float* getCamera()
{		
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

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

/* ************************************************************************* */
/* ************** I have changed the way NeHe loades the BMP *************** */
/* ************** ...because I couldn't get Glaux to work... *************** */
/* ************************************************************************* */

/* Loads A Bitmap Image */
AUX_RGBImageRec *LoadBMP(char *Filename)
{
	// File Handle
	FILE *File=NULL;

	// Make Sure A Filename Was Given
	if (!Filename)
	{
		// If Not Return NULL
		return NULL;
	}

	// Check To See If The File Exists
	File=fopen(Filename,"r");

	// Does The File Exist?
	if (File)
	{
		// Close The Handle
		fclose(File);
		
		// Load The Bitmap And Return A Pointer
		return auxDIBImageLoad(Filename);
	}

	// If Load Failed Return NULL
	return NULL;
}

/* Load Bitmaps And Convert To Textures */
int LoadGLTextures()
{
	// Status Indicator
	int Status=FALSE;

	// Create Storage Space For The Texture
	AUX_RGBImageRec *TextureImage[1];

	// Set The Pointer To NULL
	memset(TextureImage,0,sizeof(void *)*1);
	
	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Data/Crate.bmp"))
	{
		// Set The Status To TRUE
		Status=TRUE;

		// Create Three Textures
		glGenTextures(3, &texture[0]);
		
		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		
		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		
		// Create MipMapped / Trilinear Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	// If Texture Exists
	if (TextureImage[0])
	{
		// If Texture Image Exists Free The Texture Image Memory
		if (TextureImage[0]->data) free(TextureImage[0]->data);
		
		// Free The Image Structure
		free(TextureImage[0]);
	}

	// Return The Status
	return Status;
}
