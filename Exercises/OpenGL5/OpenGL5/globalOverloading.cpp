#pragma once

#ifndef globalOverloading__H__
#define globalOverloading__H__

#include "memoryManager.h"
#include <cstdlib>

void * operator new(size_t size)
{
    void * storage = MemoryManager::allocate(size);
	if(NULL == storage) {
            throw "allocation fail : no free memory";
    }
	return storage;
}


//
//template <class T>
//inline void newDelete(T & ptr)
//{
//	{assert(ptr != NULL);}
//    delete ptr; // call the destructor
//    ptr = NULL; // always remember to reset the pointer to null
//}
//
//template <class T>
//inline void deleteArray(T & ptrArray)
//{
//    {assert(ptrArray != NULL);}
//    delete [] ptrArray; // call the destructor
//    ptrArray = NULL;	// always remember to reset the pointer to null
//}
//
//template <class T>
//inline void * newMalloc(size_t size, const std::string caller) 
//{
//	void * result;
//	result = malloc(size);
//	return result;
//}

#endif