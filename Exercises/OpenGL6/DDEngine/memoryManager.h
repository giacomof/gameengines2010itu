#ifdef __DDEngine
#    define MemoryManager_D __declspec(dllexport)
#else
#    define MemoryManager_D __declspec(dllimport)
#endif

#ifndef MemoryManager__H
#define MemoryManager__H

#include "globals.h"
#include "mutexManager.h"
#include "log.h"
#include <stdlib.h>
#include <iostream>
#include <list>

using namespace std;

class MemoryManager_D MemoryManager
{
public:

	// new and delete local overload
	void * MemoryManager::operator new(size_t s);
	void MemoryManager::operator delete(void *);

	// Stack Allocator declarations	
	static unsigned int stackMarker;
	static unsigned int lastStackMarker;

	// Single Frame Allocator declarations
	static unsigned int baseSingleFrameMarker;
	static unsigned int singleFrameMarker;

	// Common variables declarations
	static unsigned int const dataToAllocate = 128 * 10000;
	static unsigned int const guardBytes = 512;

	// Log Manager
	static Log logManager; 

	// Singleton Definitions
	static MemoryManager _instance;
	static unsigned int count;

	MemoryManager(void);
	~MemoryManager(void); 
	MemoryManager(const MemoryManager & getInstance(void));   
	MemoryManager & operator=(MemoryManager & getInstance(void));
	static MemoryManager & getInstance(void);

	// newMalloc and newFree definitions
	static void * MemoryManager::newMalloc(size_t s, unsigned short typeFlag);
	static void MemoryManager::newFree(void * ptr);

	// Stack Allocator
		// used for load-and-stay-residend data
	void * MemoryManager::allocateOnStack(unsigned int s);
	void MemoryManager::freeToLastStackMarker(void);

	unsigned int MemoryManager::getStackMarker(void);
	void MemoryManager::setStackMarker(unsigned int m);

	// Single Frame Allocator
		// the data is kept for one frame only
	void * MemoryManager::allocateOnSingleFrameAllocator(unsigned int s);
	void MemoryManager::clearSingleFrameAllocator(void);

	unsigned int MemoryManager::getSingleFrameAllocatorMarker(void);
	void MemoryManager::setSingleFrameAllocatorMarker(unsigned int m);
	unsigned int MemoryManager::getBaseSingleFrameAllocatorMarker(void);
	void MemoryManager::setBaseSingleFrameAllocatorMarker(unsigned int m);

};

#endif
