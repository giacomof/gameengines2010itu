#include "log.h"

void Log::addToLog(const char * filename, const char * stringToAdd)
{
	
	FILE * pFile = fopen ( filename , "a+" );
	fseek(pFile, 0, SEEK_END);
	fputs ( stringToAdd , pFile );

}

void Log::clearLog(const char * filename)
{

	FILE * pFile = fopen ( filename , "a+" );
	fputs ( "" , pFile );

}

