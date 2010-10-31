//#pragma once
//
//#ifndef globalOverloading__H__
//#define globalOverloading__H__
//
//#include "memoryManager.h"
////#include "SDL_mutex.h"
//#include <cstdlib>
//
////static SDL_mutex * mutex_event = SDL_CreateMutex();
//
//void * operator new(size_t size, unsigned short flag)
//{
//	//AssetManager::lockMutex(mutex_event);
//
//	void * storage = MemoryManager::allocate(size);
//	if(NULL == storage) {
//            throw "allocation fail : no free memory";
//    }
//
//	//AssetManager::unlockMutex(mutex_event);
//
//	return storage;
//}
//
////void operator delete(void *m, unsigned short flag) {
////	// do nothing
////	std::cout << "I tried to delete something" << std::endl;
////}
//
//
////
////template <class T>
////inline void newDelete(T & ptr)
////{
////	{assert(ptr != NULL);}
////    delete ptr; // call the destructor
////    ptr = NULL; // always remember to reset the pointer to null
////}
////
////template <class T>
////inline void deleteArray(T & ptrArray)
////{
////    {assert(ptrArray != NULL);}
////    delete [] ptrArray; // call the destructor
////    ptrArray = NULL;	// always remember to reset the pointer to null
////}
////
////template <class T>
////inline void * newMalloc(size_t size, const std::string caller) 
////{
////	void * result;
////	result = malloc(size);
////	return result;
////}
//
//#endif