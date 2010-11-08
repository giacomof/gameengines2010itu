#include "windowManager.h"

WindowManager WindowManager::_instance;

WindowManager & WindowManager::getInstance()
{
	return _instance;
}

void WindowManager::createWindow(int w, int h, int b) 
{
	screenWidth = w;
	screenHeight = h;
	screenColorDepth = b;

	videoInfo = SDL_GetVideoInfo();
	if (!videoInfo)
	{
		exit(1);
	}

	// sets the window as Active
	isActive = TRUE;

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
		// if something has gone wrong
		// stop the render and exit the window
		exit(1);
	}

}

/* Resize the window */
int WindowManager::resizeWindow(int width, int height)
{
	screenWidth = width;
	screenHeight = height;

	GLfloat ratio;
	if (screenHeight==0)
		screenHeight = 1;

	// define the new pixel aspect
	ratio = (GLfloat)screenWidth/(GLfloat)screenHeight;
	glViewport(0,0,(GLint)screenWidth,(GLint)screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// sets the Field of View, pixel ratio, Frustum
	gluPerspective(60.0f, ratio, 0.1f, 5000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 1;
}

void WindowManager::setActive(int a) 
{
	isActive = a;
}

int WindowManager::getActive(void)
{
	return isActive;
}

void WindowManager::setTitle(const char * t, const char * i)
{
	SDL_WM_SetCaption(t, i);
}

SDL_Surface * WindowManager::setVideoMode(int w, int h, int b, Uint32 f)
{
	return SDL_SetVideoMode( w, h, b, f);
}

Uint32 WindowManager::getVideoFlags(void) 
{
	return videoFlags;
}

SDL_Surface * WindowManager::getSurface(void)
{
	return surface;
}