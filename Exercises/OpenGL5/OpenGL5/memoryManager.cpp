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

void * MemoryManager::allocate(unsigned int stackSize_bytes) 
{
	void * storage = malloc(stackSize_bytes);
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

template <class T>
void * operator new(size_t size)
{
    void *storage = malloc(size);
    if(NULL == storage) {
            throw "allocation fail : no free memory";
    }
}

template <class T>
inline void newDelete(T & ptr)
{
	{assert(ptr != NULL);}
    delete ptr; // call the destructor
    ptr = NULL; // always remember to reset the pointer to null
}

template <class T>
inline void deleteArray(T & ptrArray)
{
    {assert(ptrArray != NULL);}
    delete [] ptrArray; // call the destructor
    ptrArray = NULL;	// always remember to reset the pointer to null
}

template <class T>
inline void * newMalloc(size_t size, const std::string caller) 
{
	void * result;
	result = malloc(size);
	return result;
}