#include "mutexManager.h"


MutexManager::MutexManager(void)
{
}


MutexManager::~MutexManager(void)
{
}

void MutexManager::lockMutex( SDL_mutex * m ) 
{
	SDL_mutexP(m);
}

void MutexManager::unlockMutex( SDL_mutex * m )
{
	SDL_mutexV(m);
}