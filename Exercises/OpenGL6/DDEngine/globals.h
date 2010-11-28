#ifdef __DDEngine
#    define globals_D __declspec(dllexport)
#else
#    define globals_D __declspec(dllimport)
#endif

#ifndef globals__H__
#define globals__H__

class globals_D Globals
{

public:

	// start/stop engine variable
	static bool isStopped;

	// Debug Flags and Variables
	static bool drawDebug;
	static const int verbosityLevel = 1;

	// Shaders Flags
	static const int TEXTURE_NO_SHADING		= -2;
	static const int GREEN					= -1;
	static const int NO_TEXTURE_PHONG		= 0;
	static const int TEXTURE_PHONG			= 1;
	static const int SPECULAR_MAP			= 2;

	// Logging System Variables and Flags
		// Verbosity Level
	static const int verbosityLog	= 0;
	
		// Log Files
	static const char * MEMORYMANAGER_LOGFILE;
	static const char * MALLOC_LOGFILE;
	static const char * JAVASCRIPT_LOGFILE;

	// MemoryManager Flags
		// Type Flag
	static const unsigned short ALLOCATOR	= 0;
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

	// Start/Stop Engine Methods
	static void Globals::startEngine(void);
	static void Globals::stopEngine(void);

};

#endif