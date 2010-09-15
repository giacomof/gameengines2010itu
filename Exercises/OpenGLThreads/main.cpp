#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>

#include "messagePump.h"				// Header file for the message pump library
#include "bmp.h"						// Header File for the glaux replacement library
#include "linearAlgebraDLL.h"			// Header File for our math library

// Namespaces in use
using namespace std;
using namespace linearAlgebraDLL;

// Constants
static int const	CONSTANT_SCREEN_WIDTH	= 800;		// Window Width
static int const	CONSTANT_SCREEN_HEIGHT	= 600;		// Window Height
static int const	CONSTANT_SCREEN_DEPTH	= 32;		// Color Depth
static int const	CONSTANT_DELAY_FRAME	= 16;		// Minimum time between screen frames
static int const	CONSTANT_DELAY_LOOP		= 3;		// Minimum time between loops
static float const	CONSTANT_PI				= 3.14159f;	// Pi

// Event pump singleton
MessagePump eventPump;

// The sceen and image
SDL_Surface *surface;
GLuint image;

// Mutex list
SDL_mutex *mutex_event;
SDL_mutex *mutex_render;
SDL_mutex *mutex_scene;
SDL_mutex *mutex_camera;

// Global variable to inform the threads when to quit
bool quit = false;

/* Draw the scene */
void renderFrame (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glLoadIdentity();

	// Set the camera
	float *CamTransform = getCamera();
	
	/*
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(CamTransform);
	*/

	glPushMatrix();
	{
		/*
		glColor3f(0.0f, 1.0f, 0.0f);
		for(float i = -50; i <= 50; i += 1) {
			glBegin(GL_LINES);
				glVertex3f(-50, -5.0f, i);
				glVertex3f(50, -5.0f, i);
				glVertex3f(i, -5.0f, -50);
				glVertex3f(i, -5.0f, 50);
			glEnd();
		}
		*/

		float y = -10.0f;
		glColor3f(0.0f, 1.0f, 0.0f);
		float red, green, blue = 0;
		for(float z = -50; z <= 50; z += 1) {
			green = 0.01 * (50-z);
			for(float x = -50; x <= 50; x += 1) {
				blue = red = 0.01 * (50-x);
				glColor3f(red, green, blue);
				glBegin(GL_TRIANGLES);
					glVertex3f(x+1, y, z);
					glVertex3f(x, y, z+1);
					glVertex3f(x+1, y, z+1);
				glEnd();
			}
		}
	}
	glPopMatrix();

	// Binds the "image" texture to the OpenGL object GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, image);
	glLoadIdentity();
	
	// Swaps the buffers
	SDL_GL_SwapBuffers();
}

/* Initialize OpenGL */
int renderInitGL (void)
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

/* Resize the window */
int renderSetWindow (int width, int height)
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

/* Thread for rendering the scene */
int threadRender (void *data)
{
	// Info about the video modes available
	const SDL_VideoInfo *videoInfo;

	// Bit flags to set up SDL video modes to use
	int videoFlags;

	// Stores the current SDL event to read
	SDL_Event event;

	// Init the timestamp of the last rendered frame
	Uint32 tickFrame = 0;

	// Whether a screen frame should be rendered
	bool shouldRender = true;
	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
		exit(1);
	
	videoInfo = SDL_GetVideoInfo();
	if ( !videoInfo )
		exit(1);
	
	// Video Flags
	videoFlags = SDL_OPENGL;
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	videoFlags |= SDL_RESIZABLE;
	
	// Check for and enable HW Surface acceleration if possible
	if ( videoInfo->hw_available )
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	
	// Check for and enable HW Blit acceleration if possible
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;
	
	// Apply Video Flags and Settings
	surface = SDL_SetVideoMode(	CONSTANT_SCREEN_WIDTH, CONSTANT_SCREEN_HEIGHT, CONSTANT_SCREEN_DEPTH, videoFlags);
	if ( !surface )
		exit(1);
	
	// Try to initialise OpenGL
	if ( !renderInitGL() )
		exit(1);
	
	// resizes OpenGL window
	renderSetWindow(CONSTANT_SCREEN_WIDTH, CONSTANT_SCREEN_HEIGHT);
	
	// Disable the Windows Cursor
	ShowCursor( SDL_DISABLE ); 
	
	// Binds mouse and keyboard input to the OpenGL window
	SDL_WM_GrabInput(SDL_GRAB_ON); 
	
	// Render loop
	while(!quit)
	{
		while(SDL_PollEvent(&event))
		{
			// Lock the message pump mutex and determine whether to send as priority message
			SDL_mutexP( mutex_event );
			if ( false )
				eventPump.sendPriorityMessage(event);
			else
				eventPump.sendMessage(event);
			
			// Handle locally relevant events
			switch(event.type)
			{
			case SDL_VIDEORESIZE:
				surface = SDL_SetVideoMode(	event.resize.w, event.resize.h, CONSTANT_SCREEN_DEPTH, videoFlags);
				if (!surface)
				{
					exit(1);
				}
				renderSetWindow( event.resize.w, event.resize.h);
				break;
			default:
				break;

			// Unlock the message pump again
			SDL_mutexV( mutex_event );
			}
		}
		
		// Render a frame if it is time and should be done
		if (shouldRender && SDL_GetTicks() > (tickFrame + CONSTANT_DELAY_FRAME) )
		{
			tickFrame = SDL_GetTicks();
			renderFrame();
		}

		// Minimum delay to allow other threads to run
		SDL_Delay( CONSTANT_DELAY_LOOP );
	}

	// Show the cursor again
	ShowCursor( SDL_ENABLE );
	
	exit(0);
}

/* This thread updates the scene */
int threadUpdate (void *data)
{
	while ( !quit ) {
		// Do stuff here
		SDL_Delay( CONSTANT_DELAY_LOOP );
	}

	exit(0);
}

/* Application entry point */
int main(int argc, char *argv[])
{
	// The main threads
	SDL_Thread *thread1;
	SDL_Thread *thread2;

	// Set SDL to shut down when we quit
	atexit(SDL_Quit);

	// Create the necessary mutex
	mutex_event = SDL_CreateMutex();

	// Create thread(s) for the engine
	thread1 = SDL_CreateThread ( threadRender, NULL );
	thread2 = SDL_CreateThread ( threadUpdate, NULL );

	//wait for the threads to exit
	SDL_WaitThread ( thread1, NULL );
	SDL_WaitThread ( thread2, NULL );

	// Release all mutex
	SDL_DestroyMutex ( mutex_event );

	// Delete the event pump singleton and quit
	delete &eventPump;
	exit(0);  
}

/* Loads a bitmap image */
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

/* Load bitmaps and convert to textures */
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
