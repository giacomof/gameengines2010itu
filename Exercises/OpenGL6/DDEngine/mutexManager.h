#ifdef __DDEngine
#    define MutexManager_D __declspec(dllexport)
#else
#    define MutexManager_D __declspec(dllimport)
#endif

#ifndef MutexManager__H 
#define MutexManager__H

#include <SDL_mutex.h>

class MutexManager_D MutexManager
{
public:

	MutexManager(void);
	~MutexManager(void);

	static void MutexManager::lockMutex( SDL_mutex * m );
	static void MutexManager::unlockMutex( SDL_mutex * m );

};

#endif