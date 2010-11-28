#include "globals.h"

bool Globals::isStopped = false;
bool Globals::drawDebug = true;

const char * Globals::MEMORYMANAGER_LOGFILE    = "memorymanager.txt";
const char * Globals::MALLOC_LOGFILE		   = "malloc.txt";
const char * Globals::JAVASCRIPT_LOGFILE	   = "javaScriptLOG.txt";

void Globals::startEngine(void)
{
	isStopped = false;
}

void Globals::stopEngine(void)
{
	isStopped = true;
}