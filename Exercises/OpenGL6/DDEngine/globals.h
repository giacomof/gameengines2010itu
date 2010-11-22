#ifdef __DDEngine
#    define globals_D __declspec(dllexport)
#else
#    define globals_D __declspec(dllimport)
#endif

#ifndef globals__H__
#define globals__H__

// Debug Flags and Variables
static bool drawDebug = true;
static const int verbosityLevel = 2;

// MemoryManager Flags
	// Type Flag
static const unsigned short MANAGER		= 1;
static const unsigned short SCENEGRAPH	= 2;
static const unsigned short PHYSICS		= 3;
static const unsigned short GEOMETRY	= 4;
static const unsigned short UTILITY		= 5;
static const unsigned short SHADER		= 6;
static const unsigned short SOUND		= 7;
static const unsigned short TEXTURE		= 8;

	// Allocator Flag
static const unsigned short STACK_ALLOCATOR			= 0;
static const unsigned short SINGLE_FRAME_ALLOCATOR	= 1;

#endif