#include "memoryManager.h"

typedef unsigned int Marker;

// Static Definitions
MemoryManager MemoryManager::_instance;
unsigned int MemoryManager::count=0;
unsigned int MemoryManager::marker=0;
unsigned int MemoryManager::lastMarker=0;

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
		marker = (unsigned int) malloc(dataToAllocate);
		lastMarker = marker;
		if((void *)marker == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY" << std::endl;

		// Pool Allocator Initializations

	}
	MemoryManager::count++;
	return _instance;
}

unsigned int MemoryManager::getMarker(void) {
	return marker;
}

void MemoryManager::setMarker(unsigned int m) {
	marker = m;
}

void * MemoryManager::allocateOnStack(unsigned int s) 
{
	// In this implementation we store the marker position
	// and we add the size of the object we are trying to
	// allocate. Then we add some "guard bytes" which detect
	// if something writes beyond the boundary

	// save the last marker
	lastMarker = marker;
	// add the space required to the marker
	marker = marker+s+guardBytes;

	if(verbosityLevel >= 4) cout << "MEMORY MANAGER ALLOCATES: " << s << " bytes, plus GUARD: " << guardBytes << endl;

	// return the pointer to the position of the marker
	return (void *) marker;
}

void MemoryManager::freeToLastMarker(void) 
{
	marker = lastMarker;
}

void MemoryManager::deallocate(void * stack_ptr)
{
	free(stack_ptr);	
	stack_ptr = NULL;
}

