#include "scriptHandler.h"

// Global Context
v8::Persistent<v8::Context> ScriptHandler::g_context;
// Singleton External Definitions
ScriptHandler ScriptHandler::_instance;
short ScriptHandler::count = 0;

// Object Template
v8::Handle<v8::ObjectTemplate> ScriptHandler::lightTemplate;
v8::Handle<v8::ObjectTemplate> ScriptHandler::teapotTemplate;

ScriptHandler::ScriptHandler(void)
{
	if(count==0)
		g_context = v8::Context::New();
	count++;
	&getInstance(); 
}

ScriptHandler::~ScriptHandler(void)
{
	if(count==0)
		g_context.Dispose();
	else
		count--;
	
}

ScriptHandler & ScriptHandler::getInstance(void)
{
	return _instance;
}

// ******************************************************************
// ******************** IMPLEMENTED CALLBACKS ***********************
// ******************************************************************

// ************* CONSTRUCTORS ****************

v8::Handle<v8::Value> ScriptHandler::constructLight(const v8::Arguments &args)
{
	//start a handle scope
	v8::HandleScope handleScope;
 
	//generate a new point
	Light * light = new Light();
 
	//return the wrapped point
	return wrapLight(light);
}

v8::Handle<v8::Value> ScriptHandler::constructTeapot(const v8::Arguments &args)
{
	//start a handle scope
	v8::HandleScope handleScope;
 
	int size = args[0]->Int32Value();
	bool isWireframe = args[1]->BooleanValue();

	float posX = args[2]->Int32Value();
	float posY = args[3]->Int32Value();
	float posZ = args[4]->Int32Value();


	//generate a new point
	Teapot * teapot = new Teapot(size, isWireframe, 0);

	SceneNode * temp = new SceneNode( &Root::getInstance(), "JAVASCRIPT Teapot", teapot, Vector(posX, posY, posZ), Vector(0, 0, 0), 0.0f ); 
	Root::getInstance().addChild(temp);

	//return the wrapped point
	return wrapTeapot(teapot);
}

// ************* LOG CALLBACKS ***************

v8::Handle<v8::Value> ScriptHandler::logCallback(const v8::Arguments &args)
{
        v8::HandleScope handleScope;
        v8::Local<v8::Value> value =  args[0];
        v8::String::AsciiValue ascii(value);

        Log::addToLog(Globals::JAVASCRIPT_LOGFILE, *ascii);
        
        return value;

}

v8::Handle<v8::Value> ScriptHandler::clearLogCallback(const v8::Arguments &args) 
{
        v8::HandleScope handleScope;
        v8::Local<v8::Value> value =  args[0];
        Log::clearLog(Globals::JAVASCRIPT_LOGFILE);
        
        return value;
}

// ************ OBJECT CALLBACKS *************

v8::Handle<v8::Value> ScriptHandler::rotateSceneNode(const v8::Arguments &args)
{
	v8::HandleScope handleScope;

	v8::Local<v8::String> value = args[0]->ToString();

	int vectorX = args[1]->Int32Value();
	int vectorY = args[2]->Int32Value();
	int vectorZ = args[3]->Int32Value();
	int angle   = args[4]->Int32Value();

	v8::String::AsciiValue ascii(value);

	string nodeName = *ascii;
	
	SceneNode * tempNode = Root::getInstance().findSceneNodeByName(nodeName);

	if(tempNode != NULL)
		tempNode->rotateAboutAxis(Vector(vectorX, vectorY, vectorZ), angle);
	
	return value;
}

v8::Handle<v8::Value> ScriptHandler::translateSceneNode(const v8::Arguments &args)
{
	v8::HandleScope handleScope;

	v8::Local<v8::String> value = args[0]->ToString();

	int vectorX = args[1]->Int32Value();
	int vectorY = args[2]->Int32Value();
	int vectorZ = args[3]->Int32Value();

	v8::String::AsciiValue ascii(value);

	string nodeName = *ascii;
	
	SceneNode * tempNode = Root::getInstance().findSceneNodeByName(nodeName);

	if(tempNode != NULL)
		tempNode->translate(Vector(vectorX, vectorY, vectorZ));
	
	return value;
}

// ************ OBJECT WRAPPERS **************

v8::Handle<v8::Object> ScriptHandler::wrapLight(Light * lightToWrap) 
{
    v8::HandleScope handleScope;
 
    //create a new instance
    v8::Local<v8::Object> instance = lightTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, v8::External::New(lightToWrap));

    return handleScope.Close(instance);
}

v8::Handle<v8::Object> ScriptHandler::wrapTeapot(Teapot * teapotToWrap) 
{
    v8::HandleScope handleScope;
 
    //create a new instance
    v8::Local<v8::Object> instance = teapotTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, v8::External::New(teapotToWrap));

    return handleScope.Close(instance);
}

// ******************************************************************
// ******************** BASIC SCRIPT METHODS ************************
// ******************************************************************

v8::Handle<v8::Script> ScriptHandler::readAndCompileScript(const char * filename)
{
	v8::Handle<v8::Script> script;

	FILE * sourceFile = fopen(filename,"rt");
	if( sourceFile == NULL )
	{
		Log::addToLog(Globals::JAVASCRIPT_LOGFILE, "Failed to open file\n");
		return script;
	}

	char buffer[20480];
	int bytesRead = fread( buffer, 1, 20480, sourceFile);
	if( bytesRead == -1 )
	{
		Log::addToLog(Globals::JAVASCRIPT_LOGFILE, "Failed to read file\n");
		fclose(sourceFile);
		return script;
	}
	buffer[bytesRead]='\0';

	v8::Handle<v8::String> source = v8::String::New(buffer);
	fclose(sourceFile);

	script = v8::Script::Compile(source);
	v8::Handle<v8::Value> result = script->Run();
	
	return script;

}

v8::Persistent<v8::Function> ScriptHandler::getFunctionHandle(const char * filename, const char * functionName)
{
	v8::HandleScope handleScope;
	v8::Persistent<v8::Function> function;

	v8::Handle<v8::Script> script = readAndCompileScript(filename);

	v8::Handle<v8::String> processName = v8::String::New(functionName);
	v8::Handle<v8::Value> valueHandle = g_context->Global()->Get(processName);

	// bail if we couldn't find the function
	if( !valueHandle->IsFunction()) 
		return function;

	v8::Handle<v8::Function> funcHandle = v8::Handle<v8::Function>::Cast(valueHandle);
	function = v8::Persistent<v8::Function>::New(funcHandle);

	return function;

}

int ScriptHandler::runScript(const char *filename)
{
	v8::HandleScope handleScope;
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

	// *** Function Callbacks ***
	global->Set(v8::String::New("Log"), v8::FunctionTemplate::New(logCallback));
	global->Set(v8::String::New("ClearLog"), v8::FunctionTemplate::New(clearLogCallback));
	global->Set(v8::String::New("Rotate"), v8::FunctionTemplate::New(rotateSceneNode));
	global->Set(v8::String::New("Translate"), v8::FunctionTemplate::New(translateSceneNode));
	
	// *** Constructor Callbacks ***
	global->Set(v8::String::New("Light"), v8::FunctionTemplate::New(constructLight));
	global->Set(v8::String::New("Teapot"), v8::FunctionTemplate::New(constructTeapot));

	// *** Function Templates ***
	v8::Handle<v8::FunctionTemplate> light_template = v8::FunctionTemplate::New();
	v8::Handle<v8::FunctionTemplate> teapot_template = v8::FunctionTemplate::New();
	
	lightTemplate = light_template->InstanceTemplate();
	lightTemplate->SetInternalFieldCount(1);

	teapotTemplate = teapot_template->InstanceTemplate();
	teapotTemplate->SetInternalFieldCount(1);

	v8::Handle<v8::Context> context = v8::Context::New(NULL,global);
	g_context = v8::Persistent<v8::Context>::New(context); 
	v8::Context::Scope context_scope(g_context);

	v8::Handle<v8::Script> script = readAndCompileScript(filename);

	return 0;
}

v8::Handle<v8::Value> ScriptHandler::getResult(v8::Persistent<v8::Function> function, v8::Handle<v8::Value> * args, const int numArgs)
{
	v8::Handle<v8::Value> result = function->Call(g_context->Global(), numArgs, args);
	return result;
}
