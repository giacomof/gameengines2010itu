#define MemoryManager_D __declspec(dllimport)

#ifndef MemoryManager__H
#define MemoryManager__H

#include "assetManager.h"
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
	static void MemoryManager::freeToLastMarker(void);

	static unsigned int MemoryManager::getMarker(void);
	static void MemoryManager::setMarker(unsigned int m);
	static void MemoryManager::freeToMarker(unsigned int m);
	static void MemoryManager::clear(void);

};

#endif
