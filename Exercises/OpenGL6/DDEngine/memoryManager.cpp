#include "memoryManager.h"

// Memory Manager Declarations
typedef unsigned int Marker;
MemoryManager MemoryManager::_instance;
unsigned int MemoryManager::count = 0;
unsigned int MemoryManager::totalMemoryUsage = 0;

// Stack Allocator External Declarations
unsigned int MemoryManager::stackMarker		= 0;
unsigned int MemoryManager::lastStackMarker = 0;

// Single Frame Allocator External Declarations
unsigned int MemoryManager::singleFrameMarker	  = 0;
unsigned int MemoryManager::lastSingleFrameMarker = 0;
unsigned int MemoryManager::baseSingleFrameMarker = 0;

void * MemoryManager::operator new(size_t s) 
{
	void * storage = MemoryManager::newMalloc(s, MANAGER);
	return storage;
}

void MemoryManager::operator delete(void *) { }

MemoryManager::MemoryManager(void) { 
	&getInstance(); 
}

MemoryManager::~MemoryManager(void) {

}

MemoryManager & MemoryManager::getInstance()
{
	if(MemoryManager::count==0) {
		
		// Stack Allocator Initializations
		stackMarker = (unsigned int) MemoryManager::newMalloc(dataToAllocate, ALLOCATOR);
		lastStackMarker = stackMarker;
		if((void *)stackMarker == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY FOR THE STACK ALLOCATOR" << std::endl;

		// Single Frame Allocator Initializations
		baseSingleFrameMarker = (unsigned int) MemoryManager::newMalloc(dataToAllocate, ALLOCATOR);
		singleFrameMarker = baseSingleFrameMarker;
		if((void *)baseSingleFrameMarker == NULL) std::cout << "ERROR, NOT ENOUGH MEMORY FOR THE SINGLE FRAME ALLOCATOR" << std::endl;

		// clear the log file
		if(verbosityLog>=1) Log::clearLog(MALLOC_LOGFILE);

	}
	
	MemoryManager::count++;
	
	return _instance;
}

// ************************************************************
// ************** MALLOC AND FREE REDEFINITION ****************
// ************************************************************

void * MemoryManager::newMalloc(size_t s, unsigned short typeFlag) 
{
	if(verbosityLevel >= 4) cout << "NEW MALLOC ALLOCATES: " << s << " bytes" << endl;
	if(verbosityLog>=2) {
		Log::addToLog(MALLOC_LOGFILE, "MALLOC,");
		
		char * sizeAllocated = new char();
		char * flag			 = new char();
		itoa(s, sizeAllocated, 10);
		itoa(typeFlag, flag, 10);

		Log::addToLog(MALLOC_LOGFILE, sizeAllocated);
		Log::addToLog(MALLOC_LOGFILE, ",");
		Log::addToLog(MALLOC_LOGFILE, flag);
		Log::addToLog(MALLOC_LOGFILE, "\n");
	}
	return malloc(s);
}

void MemoryManager::newFree(void * ptr)
{
	if(verbosityLevel >= 4) cout << "NEW FREE FREES: " << ptr << endl;
	free(ptr);
}

// ************************************************************
// ***************** STACK ALLOCATOR METHODS ******************
// ************************************************************

unsigned int MemoryManager::getStackMarker(void) 
{
	return stackMarker;
}

void MemoryManager::setStackMarker(unsigned int m) 
{
	stackMarker = m;
}

void * MemoryManager::allocateOnStack(unsigned int s) 
{
	// add the size of this allocation to the max memory usage
	totalMemoryUsage += s;
	// save the last stackMarker
	lastStackMarker = stackMarker;
	// add the space required to the stackMarker
	stackMarker = stackMarker+s;

	if(verbosityLevel >= 4) cout << "STACK ALLOCATOR ALLOCATES: " << s << " bytes" << endl;

	// return the pointer to the position of the stackMarker
	return (void *) lastStackMarker;
}

void MemoryManager::freeToLastStackMarker(void) 
{
	stackMarker = lastStackMarker;
}

void MemoryManager::freeToMarker(void * ptr)
{
	stackMarker = (unsigned int)ptr;
}

// ************************************************************
// ************* SINGLE FRAME ALLOCATOR METHODS ***************
// ************************************************************

void * MemoryManager::allocateOnSingleFrameAllocator(unsigned int s) 
{
	// add the size of this allocation to the max memory usage
	totalMemoryUsage += s;

	lastSingleFrameMarker = singleFrameMarker;
	singleFrameMarker = singleFrameMarker + s;

	if(verbosityLevel >= 4) cout << "SINGLE FRAME ALLOCATOR ALLOCATES: " << s << " bytes" << endl;

	// return the pointer to the position of the stackMarker
	return (void *) lastSingleFrameMarker;
}

void MemoryManager::clearSingleFrameAllocator(void)
{
	singleFrameMarker = baseSingleFrameMarker;
	if(verbosityLevel>=4) cout << "SINGLE FRAME ALLOCATOR CLEARED" << endl;
}

unsigned int MemoryManager::getSingleFrameAllocatorMarker(void) 
{
	return singleFrameMarker;
}

void MemoryManager::setSingleFrameAllocatorMarker(unsigned int m) 
{
	singleFrameMarker = m;
}

unsigned int MemoryManager::getBaseSingleFrameAllocatorMarker(void) 
{
	return baseSingleFrameMarker;
}

void MemoryManager::setBaseSingleFrameAllocatorMarker(unsigned int m) 
{
	baseSingleFrameMarker = m;
}

void MemoryManager::logMaxMemoryUsage(void) 
{
	
	if(verbosityLog>=1) {
		
		Log::addToLog(MEMORYMANAGER_LOGFILE, "\n\n");
		Log::addToLog(MEMORYMANAGER_LOGFILE, "MAX MEMORY USAGE: ");

		char * sizeAllocated = new char();
		
		itoa(totalMemoryUsage, sizeAllocated, 10);
		Log::addToLog(MEMORYMANAGER_LOGFILE, sizeAllocated);

		}
}