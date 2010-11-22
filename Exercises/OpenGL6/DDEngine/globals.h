#ifdef __DDEngine
#    define globals_D __declspec(dllexport)
#else
#    define globals_D __declspec(dllimport)
#endif

#ifndef globals__H__
#define globals__H__

// Debug Flags and Variables
static bool drawDebug = true;
static const int verbosityLevel = 3;

// MemoryManager Flags
	// Type Flag
static unsigned const short MANAGER		= 1;
static unsigned const short SCENEGRAPH	= 2;
static unsigned const short PHYSICS		= 3;
static unsigned const short GEOMETRY	= 4;
static unsigned const short UTILITY		= 5;
static unsigned const short SHADER		= 6;
static unsigned const short SOUND		= 7;

	// Allocator Flag
static unsigned const short STACK_ALLOCATOR			= 0;
static unsigned const short SINGLE_FRAME_ALLOCATOR	= 1;

#endif