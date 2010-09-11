#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL

#include "bmp.h"						// Header File for the glaux replacement library
#include "linearAlgebraDLL.h"			// Header File for our math library

using namespace std;
using namespace linearAlgebraDLL;

static int const screenWidth		= 800;			// Window Width
static int const screenHeight		= 600;			// Window Height
static int const screenColorDepth	= 32;			// Color Depth
static int const tick				= 16;			// check timer between frames

int oldPosX, oldPosY, oldPosZ;

SDL_Surface *surface;					
GLuint image;							

// Define Lights Attributes
// *************************************
// ********** GREEN LIGHTS *************
// *************************************
GLfloat Ambient[] = { 0.0f, 255.0f, 0.0f, 1.0f};  
GLfloat Diffuse[] = {0.0f, 255.0f, 0.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 

// Camera and Movements Definitions
float rotationSpeed = 0.1f;
float camYaw, camPitch; 
int mouseStateX, mouseStateY, centerX=0, centerY=0, dX, dY, temp;
float camPosX, camPosY, camPosZ;
float camSpeed = 0.1f;
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
void Quit(int retCode);
void applyCamera();
void clampCamera();


GLuint	filter;				
GLuint	texture[3];

int main(int argc, char **argv)
{
    int videoFlags;
    int done = FALSE;
    SDL_Event event;
    const SDL_VideoInfo *videoInfo;
    int isActive = TRUE;

    if (SDL_Init(SDL_INIT_VIDEO)<0)
    {
        Quit(1);
    }

    videoInfo = SDL_GetVideoInfo();
    if (!videoInfo)
    {
        Quit(1);
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
    surface = SDL_SetVideoMode( screenWidth,
								screenHeight,
								screenColorDepth,
								videoFlags);
    if (!surface)
    {
        Quit(1);
    }

    
    if (initGL()==FALSE)
    {
        Quit(1);
    }
    
	// resizes OpenGL window
	resizeWindow(screenWidth, screenHeight);

	// Disable the Windows Cursor
    ShowCursor(SDL_DISABLE); 
	// Binds mouse and keyboard input to the OpenGL window
    SDL_WM_GrabInput(SDL_GRAB_ON); 
     
    while(!done)
    {
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
                surface = SDL_SetVideoMode( event.resize.w,
											event.resize.h,
											screenColorDepth,
											videoFlags);
                if (!surface)
                {
                    Quit(1);
                }

                resizeWindow( event.resize.w,
							  event.resize.h);
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
        if (isActive)
        {
            drawGL();
            update();
            SDL_Delay(timeLeft());
        }
    }
    ShowCursor(TRUE);
    Quit(0);
    return 0;
}

//***************************************************
void drawGL(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glLoadIdentity();
	applyCamera();
	glPushMatrix();
    {
		glColor3f(0.0f, 1.0f, 0.0f);
		for(float i = -50; i <= 50; i += 1)
		{
			glBegin(GL_LINES);
				glVertex3f(-50, -5.0f, i);
				glVertex3f(50, -5.0f, i);
				glVertex3f(i, -5.0f, -50);
				glVertex3f(i, -5.0f, 50);
			glEnd();
		}
    }
    glPopMatrix();

	// Binds the "image" texture to the OpenGL object GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, image);
	// Swaps the buffers
	glLoadIdentity();
	

    SDL_GL_SwapBuffers();
}

void update()
{
	oldPosX = camPosX;
	oldPosY = camPosY;
	oldPosZ = camPosZ;

	if (wKeyPressed==1)
    {
		/*
		camPosX += MathFunctions::floatingPointSin(camYaw)*camSpeed;
        camPosZ += MathFunctions::floatingPointCos(camYaw)*camSpeed;
        camPosY -= MathFunctions::floatingPointSin(camPitch)*camSpeed;
		*/

		camPosX += MathFunctions::floatingPointSin(camYaw)*camSpeed;
        camPosZ += MathFunctions::floatingPointCos(camYaw)*camSpeed;
        camPosY += MathFunctions::floatingPointSin(-camPitch)*camSpeed;

    }
    
    if (sKeyPressed==1)
    {
		camPosY += MathFunctions::floatingPointSin(camPitch)*camSpeed;
		camPosX -= MathFunctions::floatingPointSin(camYaw)*camSpeed;
		camPosZ -= MathFunctions::floatingPointCos(camYaw)*camSpeed;
    }
    
    if (aKeyPressed==1)
    {
		camPosX -= MathFunctions::floatingPointSin(camYaw-90)*camSpeed;
        camPosZ -= MathFunctions::floatingPointCos(camYaw-90)*camSpeed;
    }
   
    if (dKeyPressed==1)
    {
		camPosX -= MathFunctions::floatingPointSin(camYaw+90)*camSpeed;
        camPosZ -= MathFunctions::floatingPointCos(camYaw+90)*camSpeed;
    }
}

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

void keyDown(SDL_keysym *keysym)
{
    switch(keysym->sym)
    {
    case SDLK_ESCAPE:
        Quit(0);
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

void applyCamera()
{    
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	float tranM[16];
	Matrix translationMatrix = Matrix::generateAxesRotationMatrix(Vector(1.0,0.0,0.0),-camPitch).getTranspose();
	translationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);
	
	translationMatrix = Matrix::generateAxesRotationMatrix(Vector(0.0,1.0,0.0),-camYaw).getTranspose();
	translationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);

	//if(camPitch>=90||camPitch<=-90) translationMatrix = Matrix::generateTranslationMatrix(oldPosX, oldPosY, oldPosZ).getTranspose();
	//else 
	translationMatrix = Matrix::generateTranslationMatrix(camPosX, camPosY, camPosZ).getTranspose();
	translationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);

}

void clampCamera()
{
	// limits camera pitch
    if (camPitch>90.0f)
        camPitch = 90.0f;
    else if (camPitch<-90.0f)
        camPitch = -90.0f;

    
	/*
	while(camYaw<=0.0f)
        camYaw += 360.0f;
    while(camYaw>=360.0f)
        camYaw -= 360.0f;
	*/
}

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

Uint32 timeLeft(void)
{
    static Uint32 next_time = 0;
    Uint32 now;
    now = SDL_GetTicks();
    if (next_time<=now)
    {
        next_time = now + tick;
        return 0;
    }
    return (Uint32)(next_time-now);
}

void Quit(int retCode)
{
    SDL_Quit();
    exit(retCode);
}

/* ************************************************************************* */
/* ************** I have changed the way NeHe loades the BMP *************** */
/* ************** ...because I couldn't get Glaux to work... *************** */
/* ************************************************************************* */

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Data/Crate.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(3, &texture[0]);					// Create Three Textures

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

	if (TextureImage[0])								// If Texture Exists
	{
		if (TextureImage[0]->data) free(TextureImage[0]->data);				// If Texture Image Exists Free The Texture Image Memory
		
		free(TextureImage[0]);							// Free The Image Structure
	}

	return Status;										// Return The Status
}
