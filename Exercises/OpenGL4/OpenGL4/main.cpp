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
#include "Geometry.h"					// Header File for the Geometry container
#include "messagePump.h"				// Header File for the input messahe pump system
#include "md2Loader.h"					// Header File for our md2 loader

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

	Geometry bigTriangle = Geometry();
	bigTriangle.addVertex(&Point(0.0f, 0.0f, 0.0f));
	bigTriangle.addVertex(&Point(100.0f, 0.0f, 0.0f));
	bigTriangle.addVertex(&Point(0.0f, 0.0f, -100.0f));


	SceneNode plane(rootNodePtr, "Triangle Plane", &bigTriangle, 0.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	SceneNode plane2(&plane, "Triangle Plane2", &bigTriangle, 100.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	plane2.scale(1,1,1);
	SceneNode plane3(&plane2, "Triangle Plane3", &bigTriangle, 50.0f, 0.0f, 0.0f, Vector(1.0f,0.0f,0.0f), 90.0f);
	plane3.scale(1,1,1);



	while(!quit)
	{
		plane.rotateAboutAxis(Vector(0,1,0),0.2f);
		plane2.rotateAboutAxis(Vector(0,1,0),0.3f);
		plane3.rotateAboutAxis(Vector(0,1,0),0.4f);

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

	
	//Root::drawGeometry();
	rootNodePtr->drawGeometry();
	//glPopMatrix();

	// Binds the "image" texture to the OpenGL object GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, image);

	// ******************************
	// ******** DEBUG INFO **********
	// ******************************
	
	// write memory usage
	//std::cout << "memory usage " << (md2istance.GetDataSize()/1024.0f) << "kb\n";

	// render the md2 model
	md2istance.Render();
	
	// Swaps the buffers
	SDL_GL_SwapBuffers();
}
//
//void RenderFrame (int n, const struct md2_model_t *mdl)
//{
//  int i, j;
//  GLfloat s, t;
//  vec3_t v;
//  struct md2_frame_t *pframe;
//  struct md2_vertex_t *pvert;
//
//  /* Check if n is in a valid range */
//  if ((n < 0) || (n > mdl->header.num_frames - 1))
//    return;
//
//  /* Enable model's texture */
//  glBindTexture (GL_TEXTURE_2D, mdl->tex_id);
//
//  /* Draw the model */
//  glBegin (GL_TRIANGLES);
//    /* Draw each triangle */
//    for (i = 0; i < mdl->header.num_tris; ++i)
//      {
//	/* Draw each vertex */
//	for (j = 0; j < 3; ++j)
//	  {
//	    pframe = &mdl->frames[n];
//	    pvert = &pframe->verts[mdl->triangles[i].vertex[j]];
//
//	    /* Compute texture coordinates */
//	    s = (GLfloat)mdl->texcoords[mdl->triangles[i].st[j]].s / mdl->header.skinwidth;
//	    t = (GLfloat)mdl->texcoords[mdl->triangles[i].st[j]].t / mdl->header.skinheight;
//
//	    /* Pass texture coordinates to OpenGL */
//	    glTexCoord2f (s, t);
//
//	    /* Normal vector */
//	    glNormal3fv (anorms_table[pvert->normalIndex]);
//
//	    /* Calculate vertex real position */
//	    v[0] = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
//	    v[1] = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
//	    v[2] = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];
//
//	    glVertex3fv (v);
//	  }
//      }
//  glEnd ();
//}

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
	md2istance.Load("excavator.md2");

	//// ******************************
	//// ******** DEBUG INFO **********
	//// ******************************
	//
	//// write memory usage
	//std::cout << "memory usage " << (md2istance.GetDataSize()/1024.0f) << "kb\n";

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