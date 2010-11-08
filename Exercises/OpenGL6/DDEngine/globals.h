#ifdef __DDEngine
#    define globals_D __declspec(dllexport)
#else
#    define globals_D __declspec(dllimport)
#endif

#ifndef globals__H__
#define globals__H__

static bool drawDebug = true;
static const int verbosityLevel = 3;

// MemoryManager Flags
static unsigned short managersFlag = 1;
static unsigned short sceneGraphFlag = 2;
static unsigned short PhysicsFlag = 3;
static unsigned short GeometryFlag = 4;


#endif