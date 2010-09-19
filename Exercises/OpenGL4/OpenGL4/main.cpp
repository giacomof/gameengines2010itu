#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>
#include <glut.h>

#include "linearAlgebraDLL.h"			// Header File for our math library
#include "SceneNode.h"					// Header File for the SceneNode/Scenegraph
#include "Geometry.h"					// Header File for the Geometry container
#include "messagePump.h"				// Header File for the input messahe pump system
#include "md2Loader.h"					// Header File for our md2 loader
#include "pcxLoader.h"

using namespace std;
using namespace linearAlgebraDLL;

static int const screenWidth		= 800;			// Window Width
static int const screenHeight		= 600;			// Window Height
static int const screenColorDepth	= 32;			// Color Depth
static int const tick				= 16;			// Minimum time between screen frames
static int const thread_delay		= 3;			// Minimum time between loops

static float const PI = 3.14159f;					// PI definition

// Message pump used for passing Events between threads
MessagePump inputPump;								

SDL_Surface *surface;					
GLuint image;	

// Root node of the Scene Graph
Root * rootNodePtr;

md2Loader md2istance;
unsigned int md2Texture;


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
float camSpeed = 0.5f;
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

// Pointer to the function that moves the camera
float* getCamera();
// Function that restricts camera movements
void clampCamera();

GLuint	filter;
GLuint	texture[3];

// Mutex to lock variables
SDL_mutex *value_mutex;

// Defines when to stop looping
bool quit = false;

// A function to load a bitmap file and return the texture object for that texture
unsigned int MakeTexture(const char* filename) {

	unsigned int w,h,bpp;
	unsigned char* pixels;

	if(!LoadPcxImage(filename,&pixels,&w,&h,&bpp)) {
		return 0;
	}

	GLenum infmt,outfmt;
	switch(bpp) {
	case 3:
		infmt = GL_RGB;
		outfmt = GL_RGB;
		break;
	case 4:
		infmt = GL_RGBA;
		outfmt = GL_RGBA;
		break;
	case 1:
		infmt = outfmt = GL_ALPHA;
		break;
	case 2:
		infmt = outfmt = GL_RGB5_A1;
		break;
	default:
		free(pixels);
		return 0;
	}
	
	unsigned int tex_obj=0;
	glGenTextures(1,&tex_obj);

	glBindTexture (GL_TEXTURE_2D, tex_obj);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,outfmt,w,h,0,infmt,GL_UNSIGNED_BYTE,pixels);

	return tex_obj;
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

	// Move the mouse in the center of the windows before starting render
	SDL_WarpMouse((short)centerX, (short)centerY);

	// Create the root node
	rootNodePtr = new Root();

	// The updater thread
	SDL_Thread *id1;
	char *tnames[1] = { "Updater" };

	// Define the exit of the program in relation to SDL variables
	atexit(SDL_Quit);

	// Create the mutex
	value_mutex = SDL_CreateMutex();

	//create the threads
	id1 = SDL_CreateThread ( updater, tnames[0] );

	/*Geometry bigTriangle = Geometry(0);
	bigTriangle.addVertex(&Point(0.0f, 0.0f, 0.0f));
	bigTriangle.addVertex(&Point(100.0f, 0.0f, 0.0f));
	bigTriangle.addVertex(&Point(0.0f, 0.0f, -100.0f));


	SceneNode plane(rootNodePtr, "Triangle Plane", &bigTriangle, 0.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	SceneNode plane2(&plane, "Triangle Plane2", &bigTriangle, 100.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	plane2.scale(1,1,1);
	SceneNode plane3(&plane2, "Triangle Plane3", &bigTriangle, 50.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	plane3.scale(1,1,1);*/

	Geometry sunG = Geometry(1);
	sunG.setSphere(50, 30, 30);
	SceneNode sun(rootNodePtr, "Sun", &sunG, 0.0f, 0.0f, 0.0f, Vector(0.0f,0.0f,0.0f), 0.0f);

	Geometry earthG = Geometry(1);
	earthG.setSphere(20, 30, 30);
	SceneNode earth(&sun, "Earth", &earthG, 200.0f, 0.0f, 0.0f, Vector(0.0f,0.0f,0.0f), 0.0f);

	Geometry moonG = Geometry(1);
	moonG.setSphere(1, 30, 30);
	SceneNode moon(&earth, "Moon", &moonG, 100.0f, 0.0f, 0.0f, Vector(0.0f,0.0f,0.0f), 0.0f);

	Geometry doomDemon = Geometry(&md2istance);
	SceneNode demon(rootNodePtr, "Doom Demon", &doomDemon, 0.0f, 0.0f, 0.0f, Vector(0.0f,0.0f,0.0f), 0.0f);



	while(!quit)
	{
		sun.rotateAboutAxis(Vector(0,1,0),0.2f);
		earth.rotateAboutAxis(Vector(0,1,0),0.3f);
		moon.rotateAboutAxis(Vector(0,1,0),0.4f);

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

		//release the lock 
		//SDL_mutexV ( value_mutex );
		
		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}
	ShowCursor(TRUE);

	

	//wait for the threads to exit
	SDL_WaitThread ( id1, NULL );

	// Release the mutex
	SDL_DestroyMutex ( value_mutex );

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
	glBindTexture(GL_TEXTURE_2D, md2Texture);

	//Root::drawGeometry();
	rootNodePtr->drawGeometry();
	//glPopMatrix();



	// ******************************
	// ******** DEBUG INFO **********
	// ******************************
	
	// write memory usage
	//std::cout << "memory usage " << (md2istance.GetDataSize()/1024.0f) << "kb\n";

	// draw the animation
	md2istance.Update(0.03);
	
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
	
	//// loads the md2 file
	md2istance.Load("include/Cyber.md2");
	md2Texture = MakeTexture("include/cyber.pcx");

	//// ******************************
	//// ******** DEBUG INFO **********
	//// ******************************
	
	// write memory usage
	std::cout << "memory usage " << (md2istance.GetDataSize()/1024.0f) << "kb\n";
	glColor3f(1,1,1);

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
	case SDLK_0:
		md2istance.SetAnim(0);
		break;
	case SDLK_1:
		md2istance.SetAnim(1);
		break;
	case SDLK_2:
		md2istance.SetAnim(2);
		break;
	case SDLK_3:
		md2istance.SetAnim(3);
		break;
	case SDLK_4:
		md2istance.SetAnim(4);
		break;
	case SDLK_5:
		md2istance.SetAnim(5);
		break;
	case SDLK_6:
		md2istance.SetAnim(6);
		break;
	case SDLK_7:
		md2istance.SetAnim(7);
		break;
	case SDLK_8:
		md2istance.SetAnim(8);
		break;
	case SDLK_9:
		md2istance.SetAnim(9);
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
	//glPushMatrix();

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