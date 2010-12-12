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
#include "assetManager.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class scriptHandler_D ScriptHandler
{

public:

	// Variables
	static v8::Persistent<v8::Context> g_context;
	
	// Object Templates
	static v8::Handle<v8::ObjectTemplate> lightTemplate;
	static v8::Handle<v8::ObjectTemplate> teapotTemplate;
	static v8::Handle<v8::ObjectTemplate> colladaTemplate;

	// Implemented Callbacks
		//Constructor callbacks
	static v8::Handle<v8::Value> ScriptHandler::constructLight(const v8::Arguments &args);
	static v8::Handle<v8::Value> ScriptHandler::constructTeapot(const v8::Arguments &args);
	static v8::Handle<v8::Value> ScriptHandler::constructCollada(const v8::Arguments &args);

		// Wrappers
	static v8::Handle<v8::Object> ScriptHandler::wrapLight(Light * lightToWrap);
	static v8::Handle<v8::Object> ScriptHandler::wrapTeapot(Teapot * teapotToWrap);
	static v8::Handle<v8::Object> ScriptHandler::wrapCollada(ColladaInterface * colladaToWrap);

		// Log methods
	static v8::Handle<v8::Value> ScriptHandler::logCallback(const v8::Arguments &args);
	static v8::Handle<v8::Value> ScriptHandler::clearLogCallback(const v8::Arguments &args);
		
		// SceneNodes methods
	static v8::Handle<v8::Value> ScriptHandler::rotateSceneNode(const v8::Arguments &args);
	static v8::Handle<v8::Value> ScriptHandler::translateSceneNode(const v8::Arguments &args);

	// Basic Script Methods 
	int ScriptHandler::runScript(const char * filename);
	v8::Handle<v8::Script> ScriptHandler::readAndCompileScript(const char * filename);
	v8::Persistent<v8::Function> ScriptHandler::getFunctionHandle(const char * filename, const char * functionName);	
	v8::Handle<v8::Value> ScriptHandler::getResult(v8::Persistent<v8::Function> function, v8::Handle<v8::Value> *args, const int numArgs);
	
	// Singleton Definitions
	static ScriptHandler _instance;
	ScriptHandler();
	~ScriptHandler();
	ScriptHandler(const ScriptHandler & getInstance());   
	ScriptHandler & operator=(ScriptHandler & getInstance());
	static ScriptHandler & getInstance();
	static short count;

	// AssetManager
	static AssetManager * assetManager;
};

#endif

