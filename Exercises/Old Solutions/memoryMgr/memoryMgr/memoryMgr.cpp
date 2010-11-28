// memoryMgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	cout << "test 01 - Memory Manager" << endl;

	return 0;
}

void * operator new(size_t size)
{
	cout << "*** NEW OVERLOAD ***" << endl;
	void * storage = malloc(size);
	cout << "malloc at: " << storage << endl;
	return storage;
}

void operator delete(void *) {
	// do nothing
	std::cout << "I tried to delete something" << std::endl;
}