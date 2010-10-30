#pragma once

#ifndef MemoryManager__H__
#define MemoryManager__H__

#include "assetManager.h"
#include <SDL.h>
#include <stdlib.h>

class MemoryManager
{
public:

	// marker that will contain the pointer to the marker location 
	typedef unsigned int Marker;				

	// external static declaration
	static MemoryManager _instance;
	static SDL_mutex * mutex_event;
	static unsigned int count;
	static Marker mark;
	static unsigned int const dataToAllocate = 200000000; // 200Mb

	// Singleton Definitions
	MemoryManager(void);
	~MemoryManager(void); 
	MemoryManager(const MemoryManager & getInstance(void));   
	MemoryManager & operator=(MemoryManager & getInstance(void));
	static MemoryManager & getInstance(void);

	// Stack Allocator
	// used for load-and-stay-residend data
	static void * allocate(unsigned const int stackSize_bytes);
	static void MemoryManager::deallocate(void * stack_ptr);

	static Marker MemoryManager::getMarker(void);
	static void MemoryManager::setMarker(unsigned int m);
	static void MemoryManager::freeToMarker(Marker m);
	static void MemoryManager::clear(void);

 //  //Teacher's Template
 //	  typedef U32 Marker;
 //   explicit MemoryManager allocate(U32 stackSize_bytes);
 //   void * allocate (U32 size_bytes);
 //   Marker getMarker();
 //   void freeToMarker(Marker marker);
 //   void clear();

};

#endif
