#pragma once

#ifndef MemoryManager__H__
#define MemoryManager__H__

#include "assetManager.h"
#include <SDL.h>

class MemoryManager
{
public:
	static MemoryManager _instance;
	static SDL_mutex * mutex_event;
 
	// Singleton Definitions
	MemoryManager();
	~MemoryManager(); 
	MemoryManager(const MemoryManager & getInstance());   
	MemoryManager & operator=(MemoryManager & getInstance());
	static MemoryManager & getInstance();

	// methods

	// external declaration
	static unsigned int count;
};

#endif
