#include "memoryManager.h"

// Static Definitions
MemoryManager MemoryManager::_instance;
SDL_mutex * MemoryManager::mutex_event;
unsigned int MemoryManager::count=0;

MemoryManager::MemoryManager(void) { &getInstance(); }

MemoryManager::~MemoryManager(void) { }

MemoryManager & MemoryManager::getInstance()
{
	if(count==0) {
		// first esecution
		// so initialize eveything needed
		MemoryManager::mutex_event = SDL_CreateMutex();
	}
	else count++;
	return _instance;
}


