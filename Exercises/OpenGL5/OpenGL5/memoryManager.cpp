#include "memoryManager.h"

typedef unsigned int Marker;

// Static Definitions
MemoryManager MemoryManager::_instance;
unsigned int MemoryManager::count=0;
unsigned int MemoryManager::marker=0;
unsigned int MemoryManager::lastMarker=0;
list<unsigned int> * MemoryManager::markersList;

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
		markersList = new list<unsigned int>();
		std::cout << "pointer to list: " << &markersList << std::endl;

		unsigned int tempMark = (unsigned int) malloc(dataToAllocate);
		markersList->push_back(tempMark);
		if(markersList->back() == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY" << std::endl;
	}
	else MemoryManager::count++;

	return _instance;
}

unsigned int MemoryManager::getMarker(void) {
	return markersList->back();
}

void * MemoryManager::allocate(unsigned int s) 
{
	// add the space required to the marker
	markersList->push_back(markersList->back()+s);
	// return the pointer to the position of the marker
	return (void *) markersList->back();
}

void MemoryManager::freeToLastMarker(void) 
{
	markersList->pop_back();
}

void MemoryManager::deallocate(void * stack_ptr)
{
	free(stack_ptr);	
	stack_ptr = NULL;
}

