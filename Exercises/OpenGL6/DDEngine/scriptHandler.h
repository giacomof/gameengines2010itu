#ifdef __DDEngine
#    define scriptHandler_D __declspec(dllexport)
#else
#    define scriptHandler_D __declspec(dllimport)
#endif

#ifndef scriptHandler__H__
#define scriptHandler__H__

#include "v8.h"

class scriptHandler_D ScriptHandler
{
public:
	ScriptHandler(void);
	~ScriptHandler(void);
};

#endif

