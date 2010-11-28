#ifdef __DDEngine
#    define scriptHandler_D __declspec(dllexport)
#else
#    define scriptHandler_D __declspec(dllimport)
#endif

#ifndef scriptHandler__H__
#define scriptHandler__H__

#include "v8.h"

#include "log.h"
#include "globals.h"
#include "sceneObject.h"
#include "SceneNode.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace v8;

class scriptHandler_D ScriptHandler
{

public:

	// Variables
	static Persistent<Context> g_context;
	
	// Object Templates
	static Handle<ObjectTemplate> lightTemplate;
	static Handle<ObjectTemplate> teapotTemplate;

	// Implemented Callbacks
		//Constructor callbacks
	static Handle<Value> ScriptHandler::constructLight(const Arguments &args);
	static Handle<Value> ScriptHandler::constructTeapot(const Arguments &args);

		// Wrappers
	static Handle<Object> ScriptHandler::wrapLight(Light * lightToWrap);
	static Handle<Object> ScriptHandler::wrapTeapot(Teapot * teapotToWrap);

		// Log methods
	static Handle<Value> ScriptHandler::logCallback(const Arguments &args);
	static Handle<Value> ScriptHandler::clearLogCallback(const Arguments &args);
	static Handle<Value> ScriptHandler::rotateSceneNode(const Arguments &args);
	static Handle<Value> ScriptHandler::translateSceneNode(const Arguments &args);

	// Default Cons/Des
	ScriptHandler::ScriptHandler(void);
	ScriptHandler::~ScriptHandler(void);

	// Basic Script Methods 
	int ScriptHandler::runScript(const char * filename);
	Handle<Script> ScriptHandler::readAndCompileScript(const char * filename);
	Persistent<Function> ScriptHandler::getFunctionHandle(const char * filename, const char * functionName);	
	Handle<Value> ScriptHandler::getResult(Persistent<Function> function, Handle<Value> *args, const int numArgs);
	
};

#endif

