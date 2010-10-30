#include "memoryManager.h"

typedef unsigned int Marker;

// Static Definitions
MemoryManager MemoryManager::_instance;
SDL_mutex * MemoryManager::mutex_event;
unsigned int MemoryManager::count=0;
Marker MemoryManager::mark;

MemoryManager::MemoryManager(void) { &getInstance(); }

MemoryManager::~MemoryManager(void) { }

MemoryManager & MemoryManager::getInstance()
{
	if(count==0) {
		// first esecution
		// so initialize eveything needed
		MemoryManager::mutex_event = SDL_CreateMutex();
		// allocate data
		void * stackAllocated = malloc(dataToAllocate);
		if(stackAllocated == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY" << std::endl;
	}
	else count++;
	return _instance;
}

Marker MemoryManager::getMarker(void) {
	return mark;
}

void MemoryManager::setMarker(unsigned int m) {
	mark = m;
}

void * MemoryManager::allocate(unsigned int s) 
{
	void * storage = malloc(s);
	if(NULL == storage) {
        throw "allocation fail : no free memory";
    }

	return storage;
}

void MemoryManager::deallocate(void * stack_ptr)
{
	free(stack_ptr);	
	stack_ptr = NULL;
}

