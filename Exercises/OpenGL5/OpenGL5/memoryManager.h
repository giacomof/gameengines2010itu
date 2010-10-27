#pragma once

#ifndef MemoryManager__H__
#define MemoryManager__H__

#include "assetManager.h"
#include <SDL.h>
#include <stdlib.h>

class MemoryManager
{
public:

	// external static declaration
	static MemoryManager _instance;
	static SDL_mutex * mutex_event;
	static unsigned int count;

	// Singleton Definitions
	MemoryManager(void);
	~MemoryManager(void); 
	MemoryManager(const MemoryManager & getInstance(void));   
	MemoryManager & operator=(MemoryManager & getInstance(void));
	static MemoryManager & getInstance(void);

	// Stack Allocator
	// used for load-and-stay-residend data
	typedef unsigned long Marker;				// marker that will contain the pointer to the marker location 

	MemoryManager * MemoryManager::allocate(unsigned long stackSize_bytes);
	Marker MemoryManager::getMarker(void);
	void MemoryManager::freeToMarker(Marker marker);
	void MemoryManager::clear(void);

	// Teacher's Template
 //	 typedef U32 Marker;
 //   explicit MemoryManager allocate(U32 stackSize_bytes);
 //   void * allocate (U32 size_bytes);
 //   Marker getMarker();
 //   void freeToMarker(Marker marker);
 //   void clear();

};

#endif
