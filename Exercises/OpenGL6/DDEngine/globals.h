#ifdef __DDEngine
#    define globals_D __declspec(dllexport)
#else
#    define globals_D __declspec(dllimport)
#endif

#ifndef globals__H__
#define globals__H__

static bool drawDebug = true;
static const int verbosityLevel = 4;

// MemoryManager Flags
	// Type Flag
static unsigned short MANAGER	 = 1;
static unsigned short SCENEGRAPH = 2;
static unsigned short PHYSICS	 = 3;
static unsigned short GEOMETRY	 = 4;
static unsigned short UTILITY	 = 5;

	// Allocator Flag
static unsigned short AUTO_ALLOCATOR  = 0;
static unsigned short STACK_ALLOCATOR = 1;
static unsigned short POOL_ALLOCATOR  = 2;



#endif