#ifdef __DDEngine
#    define log_D __declspec(dllexport)
#else
#    define log_D __declspec(dllimport)
#endif

#ifndef log__H__
#define log__H__

#include <stdio.h>

using namespace std;

class log_D Log
{

public:
	
	static void Log::addToLog(const char * filename, const char * stringToAdd);
	static void Log::clearLog(const char * filename);

};

#endif