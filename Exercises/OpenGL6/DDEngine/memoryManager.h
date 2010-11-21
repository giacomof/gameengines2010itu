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

class MemoryManager_D MemoryManager
{
public:

	// new and delete overload
	void * MemoryManager::operator new(size_t s);
	void MemoryManager::operator delete(void *);

	// external static declaration
	static MemoryManager _instance;
	static unsigned int count;
	static unsigned int marker;
	static unsigned int lastMarker;
	static unsigned int const dataToAllocate = 256 * 10000; // 256Mb
	static unsigned int const guardBytes = 510;

	// Singleton Definitions
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

};

#endif
