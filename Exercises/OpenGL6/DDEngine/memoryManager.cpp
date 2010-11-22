#include "memoryManager.h"

typedef unsigned int Marker;

// Static Definitions
MemoryManager MemoryManager::_instance;
unsigned int MemoryManager::count=0;
unsigned int MemoryManager::stackMarker=0;
unsigned int MemoryManager::lastStackMarker=0;

void * MemoryManager::operator new(size_t s) {
	
	void * storage = malloc(s);
	return storage;
	
}

void MemoryManager::operator delete(void *){ }

MemoryManager::MemoryManager(void) { 
	&getInstance(); 
}

MemoryManager::~MemoryManager(void) { }

MemoryManager & MemoryManager::getInstance()
{
	if(MemoryManager::count==0) {
		// Stack Allocator Initializations
		stackMarker = (unsigned int) malloc(dataToAllocate);
		lastStackMarker = stackMarker;
		if((void *)stackMarker == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY" << std::endl;

		// Pool Allocator Initializations

	}
	MemoryManager::count++;
	return _instance;
}

unsigned int MemoryManager::getMarker(void) {
	return stackMarker;
}

void MemoryManager::setMarker(unsigned int m) {
	stackMarker = m;
}

void * MemoryManager::allocateOnStack(unsigned int s) 
{
	// In this implementation we store the stackMarker position
	// and we add the size of the object we are trying to
	// allocate. Then we add some "guard bytes" which detect
	// if something writes beyond the boundary

	// save the last stackMarker
	lastStackMarker = stackMarker;
	// add the space required to the stackMarker
	stackMarker = stackMarker+s+guardBytes;

	if(verbosityLevel >= 4) cout << "MEMORY MANAGER ALLOCATES: " << s << " bytes, plus GUARD: " << guardBytes << endl;

	// return the pointer to the position of the stackMarker
	return (void *) stackMarker;
}

void MemoryManager::freeToLastMarker(void) 
{
	stackMarker = lastStackMarker;
}

void MemoryManager::deallocate(void * stack_ptr)
{
	free(stack_ptr);	
	stack_ptr = NULL;
}

