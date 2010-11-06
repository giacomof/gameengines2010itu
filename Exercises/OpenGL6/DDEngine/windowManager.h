#define WindowManager_D __declspec(dllimport)

#ifndef WindowManager__H
#define WindowManager__H

#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL

class WindowManager_D WindowManager
{
public:

	static WindowManager _instance;

	WindowManager(void) { &getInstance(); };
	~WindowManager(void) {};

	// Singleton Methods
	WindowManager(const WindowManager & getInstance());   
	WindowManager & operator=(WindowManager & getInstance());
	static WindowManager & getInstance();

	void WindowManager::createWindow(int w, int h, int b);
	int WindowManager::resizeWindow(int width, int height);
	void WindowManager::setActive(int a);
	int WindowManager::getActive(void);
	void WindowManager::setTitle(const char * t, const char * i);
	SDL_Surface * WindowManager::setVideoMode(int w, int h, int b, Uint32 f);
	Uint32 WindowManager::getVideoFlags(void);
	SDL_Surface * WindowManager::getSurface(void);

	int screenWidth;			// Window Width
	int screenHeight;			// Window Height
	int screenColorDepth;		// Color Depth
	int isActive;				// is the window Active?
	int videoFlags;				// property of the window
	
	SDL_Surface * surface;		// Pointer to SDL rendering surface
	const SDL_VideoInfo *videoInfo; // information retrieved from pc hardware

};

#endif