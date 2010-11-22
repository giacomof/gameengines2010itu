#ifdef __DDEngine
#    define MemoryManager_D __declspec(dllexport)
#else
#    define MemoryManager_D __declspec(dllimport)
#endif

#ifndef MemoryManager__H
#define MemoryManager__H

#include "assetManager.h"
#include "globals.h"
#include <stdlib.h>
#include <list>

class MemoryManager_D MemoryManager
{
public:

	// new and delete local overload
	void * MemoryManager::operator new(size_t s);
	void MemoryManager::operator delete(void *);

	// Stack Allocator declarations	
	static unsigned int stackMarker;
	static unsigned int lastStackMarker;
	static unsigned int const guardBytes = 512;

	// Single Frame Allocator declarations


	// Common variables declarations
	static unsigned int const dataToAllocate = 128 * 10000; // 128Mb

	// Singleton Definitions
	static MemoryManager _instance;
	static unsigned int count;

	MemoryManager(void);
	~MemoryManager(void); 
	MemoryManager(const MemoryManager & getInstance(void));   
	MemoryManager & operator=(MemoryManager & getInstance(void));
	static MemoryManager & getInstance(void);

	// Stack Allocator
		// used for load-and-stay-residend data
	void * MemoryManager::allocateOnStack(unsigned int s);
	void MemoryManager::deallocate(void * stack_ptr);
	void MemoryManager::freeToLastMarker(void);

	unsigned int MemoryManager::getMarker(void);
	void MemoryManager::setMarker(unsigned int m);
	void MemoryManager::freeToMarker(unsigned int m);
	void MemoryManager::clear(void);

	// Single Frame Allocator
		// the data is kept for one frame only



};

#endif
