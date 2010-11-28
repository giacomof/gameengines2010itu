#include "scriptHandler.h"

// Global Context
Persistent<Context> ScriptHandler::g_context;
// Singleton External Definitions
ScriptHandler ScriptHandler::_instance;
short ScriptHandler::count = 0;

// Object Template
Handle<ObjectTemplate> ScriptHandler::lightTemplate;
Handle<ObjectTemplate> ScriptHandler::teapotTemplate;

ScriptHandler::ScriptHandler(void)
{
	if(count==0)
		g_context = Context::New();
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

Handle<Value> ScriptHandler::constructLight(const Arguments &args)
{
	//start a handle scope
	HandleScope handleScope;
 
	//generate a new point
	Light * light = new Light();
 
	//return the wrapped point
	return wrapLight(light);
}

Handle<Value> ScriptHandler::constructTeapot(const Arguments &args)
{
	//start a handle scope
	HandleScope handleScope;
 
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

Handle<Value> ScriptHandler::logCallback(const Arguments &args)
{
        HandleScope handleScope;
        Local<Value> value =  args[0];
        String::AsciiValue ascii(value);

        Log::addToLog(Globals::JAVASCRIPT_LOGFILE, *ascii);
        
        return value;

}

Handle<Value> ScriptHandler::clearLogCallback(const Arguments &args) 
{
        HandleScope handleScope;
        Local<Value> value =  args[0];
        Log::clearLog(Globals::JAVASCRIPT_LOGFILE);
        
        return value;
}

// ************ OBJECT CALLBACKS *************

Handle<Value> ScriptHandler::rotateSceneNode(const Arguments &args)
{
	HandleScope handleScope;

	Local<String> value = args[0]->ToString();

	int vectorX = args[1]->Int32Value();
	int vectorY = args[2]->Int32Value();
	int vectorZ = args[3]->Int32Value();
	int angle   = args[4]->Int32Value();

	String::AsciiValue ascii(value);

	string nodeName = *ascii;
	
	SceneNode * tempNode = Root::getInstance().findSceneNodeByName(nodeName);

	if(tempNode != NULL)
		tempNode->rotateAboutAxis(Vector(vectorX, vectorY, vectorZ), angle);
	
	return value;
}

Handle<Value> ScriptHandler::translateSceneNode(const Arguments &args)
{
	HandleScope handleScope;

	Local<String> value = args[0]->ToString();

	int vectorX = args[1]->Int32Value();
	int vectorY = args[2]->Int32Value();
	int vectorZ = args[3]->Int32Value();

	String::AsciiValue ascii(value);

	string nodeName = *ascii;
	
	SceneNode * tempNode = Root::getInstance().findSceneNodeByName(nodeName);

	if(tempNode != NULL)
		tempNode->translate(Vector(vectorX, vectorY, vectorZ));
	
	return value;
}

// ************ OBJECT WRAPPERS **************

Handle<Object> ScriptHandler::wrapLight(Light * lightToWrap) 
{
    HandleScope handleScope;
 
    //create a new instance
    Local<Object> instance = lightTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, External::New(lightToWrap));

    return handleScope.Close(instance);
}

Handle<Object> ScriptHandler::wrapTeapot(Teapot * teapotToWrap) 
{
    HandleScope handleScope;
 
    //create a new instance
    Local<Object> instance = teapotTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, External::New(teapotToWrap));

    return handleScope.Close(instance);
}

// ******************************************************************
// ******************** BASIC SCRIPT METHODS ************************
// ******************************************************************

Handle<Script> ScriptHandler::readAndCompileScript(const char * filename)
{
	Handle<Script> script;

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

	Handle<String> source = String::New(buffer);
	fclose(sourceFile);

	script = Script::Compile(source);
	Handle<Value> result = script->Run();
	
	return script;

}

Persistent<Function> ScriptHandler::getFunctionHandle(const char * filename, const char * functionName)
{
	HandleScope handleScope;
	Persistent<Function> function;

	Handle<Script> script = readAndCompileScript(filename);

	Handle<String> processName = String::New(functionName);
	Handle<Value> valueHandle = g_context->Global()->Get(processName);

	// bail if we couldn't find the function
	if( !valueHandle->IsFunction()) 
		return function;

	Handle<Function> funcHandle = Handle<Function>::Cast(valueHandle);
	function = Persistent<Function>::New(funcHandle);

	return function;

}

int ScriptHandler::runScript(const char *filename)
{
	HandleScope handleScope;
	Handle<ObjectTemplate> global = ObjectTemplate::New();

	// *** Function Callbacks ***
	global->Set(String::New("Log"), FunctionTemplate::New(logCallback));
	global->Set(String::New("ClearLog"), FunctionTemplate::New(clearLogCallback));
	global->Set(String::New("Rotate"), FunctionTemplate::New(rotateSceneNode));
	global->Set(String::New("Translate"), FunctionTemplate::New(translateSceneNode));
	
	// *** Constructor Callbacks ***
	global->Set(String::New("Light"), FunctionTemplate::New(constructLight));
	global->Set(String::New("Teapot"), FunctionTemplate::New(constructTeapot));

	// *** Function Templates ***
	Handle<FunctionTemplate> light_template = FunctionTemplate::New();
	Handle<FunctionTemplate> teapot_template = FunctionTemplate::New();
	
	lightTemplate = light_template->InstanceTemplate();
	lightTemplate->SetInternalFieldCount(1);

	teapotTemplate = teapot_template->InstanceTemplate();
	teapotTemplate->SetInternalFieldCount(1);

	Handle<Context> context = Context::New(NULL,global);
	g_context = Persistent<Context>::New(context); 
	Context::Scope context_scope(g_context);

	Handle<Script> script = readAndCompileScript(filename);

	return 0;
}

Handle<Value> ScriptHandler::getResult(Persistent<Function> function, Handle<Value> *args, const int numArgs)
{
	Handle<Value> result = function->Call(g_context->Global(), numArgs, args);
	return result;
}
