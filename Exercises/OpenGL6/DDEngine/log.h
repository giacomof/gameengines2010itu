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
	
	Log(void);
	~Log(void);

	void addToLog(const char * filename, const char * stringToAdd);
	void clearLog(const char * filename);

};

#endif