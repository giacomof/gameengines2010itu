#ifdef __DDEngine
#    define DDEngine_D __declspec(dllexport)
#else
#    define DDEngine_D __declspec(dllimport)
#endif

#ifndef DDEngine__H 
#define DDEngine__H

#include <iostream>						// Header File For the basic Input/Output system
#include <windows.h>					// Header File For Windows
#include <stdio.h>						// Header File For Standard Input/Output
#include <stdlib.h>						// Header File For the STD library
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>					// Header File for SDL thread library
#include <SDL_audio.h>					// Header File for SDL audio library
#include <glut.h>						// Header File for glut commands
#include <btBulletDynamicsCommon.h>		// Header File for Bullet Physics

#include "memoryManager.h"				// Header File for our Memory Manager
#include "linearAlgebraDLL.h"			// Header File for our math library
#include "sceneNode.h"					// Header File for the SceneNode/Scenegraph
#include "sceneObject.h"				// Header File for the SceneObject container
#include "md2File.h"					// Header File for our md2 loader
#include "colladaFile.h"				// Header File for out COLLADA loader
#include "assetManager.h"				// Header File for our Asset Manager
#include "inputManager.h"				// Header File for our Input Manager
#include "frameClock.h"					// Header File for our Clock
#include "sound.h"						// Header File for our Sound
#include "windowManager.h"				// Header File for our Window Manager
#include "debugDraw.h"					// Header File for our Debug Drawer 

using namespace std;
using namespace linearAlgebraDLL;

class DDEngine
{
public:
	DDEngine(void);
	~DDEngine(void);
};

#endif;