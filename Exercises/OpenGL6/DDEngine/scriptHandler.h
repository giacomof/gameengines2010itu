#ifdef __DDEngine
#    define scriptHandler_D __declspec(dllexport)
#else
#    define scriptHandler_D __declspec(dllimport)
#endif

#ifndef scriptHandler__H__
#define scriptHandler__H__

#include "v8.h"
#include <string>
#include <iostream>

using namespace std;
using namespace v8;

class scriptHandler_D ScriptHandler
{

public:

	// Variables
	Persistent<Context> g_context;

	// Default Cons/Des
	ScriptHandler::ScriptHandler(void);
	ScriptHandler::~ScriptHandler(void);

	// Methods
	Handle<Script> ScriptHandler::readAndCompileScript(const char * filename);

};

#endif

