#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output

#include <math.h>
#include <iostream>

#include <stdlib.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "bmp.h"			// Header File for the glaux replacement library
#include "linearAlgebra.h" // Header File for our math library
#include "customConstants.h"

using namespace std;



SDL_Surface *surface;

GLuint textures[TEXTURECOUNT]; 
float viewRotate = 0.0f; 
int Frames,CurFrame=0;
char Text[256];
GLfloat Ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};  
GLfloat Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  
GLfloat Position[] = {10.0f, 190.0f, 10.0f, 1.0f}; 
GLfloat Position1[] = {0.0f, 190.0f, 0.0f, 1.0f};  
GLfloat Position2[] = {-10.0f, 190.0f, -10.0f, 1.0f};  
GLfloat Position3[] = {0.0f, 190.0f, 0.0f, 10.0f}; 


float ANGLE_SENSITIVITY = 0.04f;
float camYaw; 
float camPitch; 
int mouseStateX, mouseStateY, centerX=0, centerY=0, dX, dY, temp;
float camPosX, camPosY, camPosZ;
float camSpeed = 0.1f;
int wKeyTyped = 0, sKeyTyped = 0, aKeyTyped = 0, dKeyTyped = 0;

GLuint image;


int initGL(void);
void drawGL(void);
void handleKeyPress(SDL_keysym *keysym);
void update();
Uint32 timeLeft(void);
GLuint loadTexture(char* texName);
int resizeWindow(int width, int height);
void Quit(int retCode);
void applyCamera();
void clampCamera();

GLuint	filter;				// Which Filter To Use
GLuint	texture[3];			// Storage For 3 Textures

int main(int argc, char **argv)
{
    cout<<"Program ehellee\n";

    int videoFlags;
    int done = FALSE;
    SDL_Event event;
    const SDL_VideoInfo *videoInfo;
    int isActive = TRUE;

    if (SDL_Init(SDL_INIT_VIDEO)<0)
    {
        cout<<"video amjiltgui"<<endl;
        Quit(1);
    }

    videoInfo = SDL_GetVideoInfo();
    if (!videoInfo)
    {
        cout<<"amjiltgui"<<endl;
        Quit(1);
    }

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

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    surface = SDL_SetVideoMode(    SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SCREEN_BPP,
                                videoFlags);
    if (!surface)
    {
        cout<<"surface amjiltgui"<<endl;
        Quit(1);
    }

    
    if (initGL()==FALSE)
    {
        cout<<"amjiltgui"<<endl;
        Quit(1);
    }
    resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    ShowCursor(SDL_DISABLE); 
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
                surface = SDL_SetVideoMode(
                    event.resize.w,
                    event.resize.h,
                    SCREEN_BPP,
                    videoFlags);
                if (!surface)
                {
                    cout<<"Amjiltgui"<<endl;
                    Quit(1);
                }
                resizeWindow(
                    event.resize.w,
                    event.resize.h
                    );
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    wKeyTyped = 1;
                    break;
                case SDLK_s:
                    sKeyTyped = 1;
                    break;
                case SDLK_a:
                    aKeyTyped = 1;
                    break;
                case SDLK_d:
                    dKeyTyped = 1;
                    break;
                default:
                    break;
                }
                handleKeyPress(&event.key.keysym);
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    wKeyTyped = 0;
                    break;
                case SDLK_s:
                    sKeyTyped = 0;
                    break;
                case SDLK_a:
                    aKeyTyped = 0;
                    break;
                case SDLK_d:
                    dKeyTyped = 0;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                
                SDL_GetMouseState(&mouseStateX, &mouseStateY);            
                cout<<mouseStateX<<" "<<mouseStateY<<endl;
                dX = (int)mouseStateX - centerX;
                dY = (int)mouseStateY - centerY;
               
                cout<<dX<<" "<<dY<<endl;
                camYaw -= ANGLE_SENSITIVITY * dX;
                camPitch -= ANGLE_SENSITIVITY * dY;
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
    ShowCursor(SDL_ENABLE);
    Quit(0);
    return 0;
}

//***************************************************
void drawGL(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glLoadIdentity();


    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    glLightfv(GL_LIGHT1, GL_POSITION, Position1);
    glLightfv(GL_LIGHT2, GL_POSITION, Position2);
    glLightfv(GL_LIGHT3, GL_POSITION, Position3);

    applyCamera();

    glPushMatrix();
    {

    glColor3f(0.0f,1.0f,1.0f);
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

    glBindTexture(GL_TEXTURE_2D, image);
    glPushMatrix();
    {

    glTranslatef(0.0f, -5.0f, -25.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);    

    glRotatef(viewRotate, 0.0f, 0.0f, 1.0f);
    glScalef(0.2f,0.2f,0.2f);

    
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(0.0f, -5.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.6f,0.6f,0.6f);

    }
    glPopMatrix();

    SDL_GL_SwapBuffers();
}

void update()
{
    if (CurFrame>Frames) 
        CurFrame=0;

    if (viewRotate++>360.0f)
        viewRotate -= 360.0f;
  
    if (wKeyTyped==1)
    {
        float xRotRad, yRotRad;
        yRotRad = (camYaw/180*NUM_PI);
        xRotRad = (camPitch/180*NUM_PI);
        camPosX += float(sin(yRotRad))*camSpeed;
        camPosZ += float(cos(yRotRad))*camSpeed;
        camPosY -= float(sin(xRotRad))*camSpeed;
    }
    
    if (sKeyTyped==1)
    {
        float xRotRad, yRotRad;
        yRotRad = (camYaw/180*NUM_PI);
        xRotRad = (camPitch/180*NUM_PI);
        camPosX -= float(sin(yRotRad))*camSpeed;
        camPosZ -= float(cos(yRotRad))*camSpeed;
        camPosY += float(sin(xRotRad))*camSpeed;
    }
    
    if (aKeyTyped==1)
    {
        float yRotRad;
        yRotRad = (camYaw/180*NUM_PI-NUM_PI/2);        
        camPosX -= float(sin(yRotRad))*camSpeed;
        camPosZ -= float(cos(yRotRad))*camSpeed;        
    }
   
    if (dKeyTyped==1)
    {
        float yRotRad;
        yRotRad = (camYaw/180*NUM_PI+NUM_PI/2);        
        camPosX -= float(sin(yRotRad))*camSpeed;
        camPosZ -= float(cos(yRotRad))*camSpeed;        
    }
}

int initGL(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    glLoadIdentity();
    glFrustum(    -0.5f,
                0.5f,
                -0.5f*(float)(SCREEN_HEIGHT/SCREEN_WIDTH),
                0.5f*(float)(SCREEN_HEIGHT/SCREEN_WIDTH),
                1.0f,
                500.0f);
    glMatrixMode(GL_MODELVIEW);

    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    glShadeModel(GL_SMOOTH);
    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    
    glEnable(GL_LIGHT1);    
    glLightfv(GL_LIGHT1, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
    glEnable(GL_LIGHT2);    
    glLightfv(GL_LIGHT2, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, Diffuse);
    glEnable(GL_LIGHT3);    
    glLightfv(GL_LIGHT3, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, Diffuse);

   
    centerX = SCREEN_WIDTH/2;
    centerY = SCREEN_HEIGHT/2;

    return TRUE;
}

void handleKeyPress(SDL_keysym *keysym)
{
    switch(keysym->sym)
    {
    case SDLK_ESCAPE:
        Quit(0);
        break;
    case SDLK_F1:
        cout<<"Shiljuullee"<<endl;
        SDL_WM_ToggleFullScreen(surface);
        break;    
    default:
        break;
    }
    return;
}

void applyCamera()
{    
    glRotatef(-camPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(camPosX, camPosY, camPosZ);
}

void clampCamera()
{
    if (camPitch>90.0f)
        camPitch = 90.0f;
    else if (camPitch<-90.0f)
        camPitch = -90.0f;

    while(camYaw<=0.0f)
        camYaw += 360.0f;
    while(camYaw>=360.0f)
        camYaw -= 360.0f;
}

int resizeWindow(int width, int height)
{
    GLfloat ratio;
    if (height==0)
        height = 1;
    ratio = (GLfloat)width/(GLfloat)height;

    glViewport(0,0,(GLint)width,(GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

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
        next_time = now + TICK_INTERVAL;
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
