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
	if(count==0) {
		marker = lastMarker = (unsigned int) malloc(dataToAllocate);
		if((void *)marker == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY" << std::endl;
	}
	else count++;
	return _instance;
}

unsigned int MemoryManager::getMarker(void) {
	return marker;
}

void MemoryManager::setMarker(unsigned int m) {
	marker = m;
}

void * MemoryManager::allocate(unsigned int s) 
{
	// save the last marker
	lastMarker = marker;
	// add the space required to the marker
	marker = marker+s;
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

