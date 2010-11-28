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
#include "log.h"
#include "globals.h"
#include "windowManager.h"

using namespace std;
using namespace v8;

class scriptHandler_D ScriptHandler
{

public:

	// Variables
	static Persistent<Context> g_context;

	// Default Cons/Des
	ScriptHandler::ScriptHandler(void);
	ScriptHandler::~ScriptHandler(void);

	// Scripting Methods
	Handle<Script> ScriptHandler::readAndCompileScript(const char * filename);
	Persistent<Function> ScriptHandler::GetFunctionHandle(const char * filename, const char * functionName);
	void ScriptHandler::runScript(const char * filename, const char * function);

	Handle<Context> ScriptHandler::getContext(void);
	Local<FunctionTemplate> ScriptHandler::makeStaticCallableFunc(InvocationCallback func);
    Local<External> ScriptHandler::classPtrToExternal();

	// Implemented Callbacks
		// Log methods
	static Handle<Value> ScriptHandler::LogCallback(const Arguments &args);
	static Handle<Value> ScriptHandler::clearLogCallback(const Arguments &args);

};

#endif

