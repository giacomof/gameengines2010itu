#include "scriptHandler.h"

// Global Context
v8::Persistent<v8::Context> ScriptHandler::g_context;
AssetManager * ScriptHandler::assetManager;

// Singleton External Definitions
ScriptHandler ScriptHandler::_instance;
short ScriptHandler::count = 0;

// Object Templates
v8::Handle<v8::ObjectTemplate> ScriptHandler::lightTemplate;
v8::Handle<v8::ObjectTemplate> ScriptHandler::teapotTemplate;
v8::Handle<v8::ObjectTemplate> ScriptHandler::colladaTemplate;

ScriptHandler::ScriptHandler(void)
{
	if(count==0) {
		g_context = v8::Context::New();
		assetManager = new AssetManager();
	}
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
	// get the local scope
	v8::HandleScope handleScope;
 
	// instantiate a new light
	Light * light = new Light();
 
	// return the wrapped light
	return wrapLight(light);
}

v8::Handle<v8::Value> ScriptHandler::constructTeapot(const v8::Arguments &args)
{

	v8::HandleScope handleScope;
 
	int size = args[0]->Int32Value();
	bool isWireframe = args[1]->BooleanValue();

	float posX = args[2]->Int32Value();
	float posY = args[3]->Int32Value();
	float posZ = args[4]->Int32Value();



	Teapot * teapot = new Teapot(size, isWireframe, 0);

	SceneNode * temp = new SceneNode( &Root::getInstance(), "JAVASCRIPT Teapot", teapot, Vector(posX, posY, posZ), Vector(0, 0, 0), 0.0f ); 
	Root::getInstance().addChild(temp);


	return wrapTeapot(teapot);
}

v8::Handle<v8::Value> ScriptHandler::constructCollada(const v8::Arguments &args)
{

	v8::HandleScope handleScope;

	char *filepath;		string filepathString;
	char *name;			string nameString;
	char *texturePath;	string texturePathString;
	char *textureName;	string textureNameString;

	v8::Local<v8::String> colladaFilepath   = args[0]->ToString();
	v8::Local<v8::String> colladaName		= args[1]->ToString();
	v8::Local<v8::String> textureFilepath   = args[2]->ToString();
	v8::Local<v8::String> textureFileName   = args[3]->ToString();

	v8::String::AsciiValue stringPtr1(colladaFilepath);
	filepathString = *stringPtr1;
	v8::String::AsciiValue stringPtr2(colladaName);
	nameString = *stringPtr2;
	v8::String::AsciiValue stringPtr3(textureFilepath);
	texturePathString = *stringPtr3;
	v8::String::AsciiValue stringPtr4(textureFileName);
	textureNameString = *stringPtr4;

	filepath	= (char*)filepathString.c_str();
	name		= (char*)nameString.c_str();
	texturePath = (char*)texturePathString.c_str();
	textureName = (char*)textureNameString.c_str();
	
	float posX = args[4]->Int32Value();
	float posY = args[5]->Int32Value();
	float posZ = args[6]->Int32Value();
	
	assetManager->loadCollada(filepath, name);
	assetManager->loadTexture(texturePath, textureName);
	
	ColladaInterface * collada = new ColladaInterface(assetManager->getColladaMesh(name), Globals::TEXTURE_PHONG, assetManager->getTexture(textureName) );

	SceneNode * temp = new SceneNode( &Root::getInstance(), "JAVASCRIPT Collada", collada, Vector(posX, posY, posZ), Vector(0, 0, 0), 0.0f ); 
	Root::getInstance().addChild(temp);


	return wrapCollada(collada);
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
 
    // create a new instance
    v8::Local<v8::Object> instance = lightTemplate->NewInstance();
 
    // set internal field on instance
    instance->SetInternalField(0, v8::External::New(lightToWrap));

    return handleScope.Close(instance);
}

v8::Handle<v8::Object> ScriptHandler::wrapTeapot(Teapot * teapotToWrap) 
{
    v8::HandleScope handleScope;
 
    // create a new instance
    v8::Local<v8::Object> instance = teapotTemplate->NewInstance();
 
    // set internal field on instance
    instance->SetInternalField(0, v8::External::New(teapotToWrap));

    return handleScope.Close(instance);
}

v8::Handle<v8::Object> ScriptHandler::wrapCollada(ColladaInterface * colladaToWrap) 
{
    v8::HandleScope handleScope;
 
    // create a new instance
    v8::Local<v8::Object> instance = colladaTemplate->NewInstance();
 
    // set internal field on instance
    instance->SetInternalField(0, v8::External::New(colladaToWrap));

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
	global->Set(v8::String::New("Collada"), v8::FunctionTemplate::New(constructCollada));

	// *** Function Templates ***
	v8::Handle<v8::FunctionTemplate> light_template = v8::FunctionTemplate::New();
	v8::Handle<v8::FunctionTemplate> teapot_template = v8::FunctionTemplate::New();
	v8::Handle<v8::FunctionTemplate> collada_template = v8::FunctionTemplate::New();
	
	lightTemplate = light_template->InstanceTemplate();
	lightTemplate->SetInternalFieldCount(1);

	teapotTemplate = teapot_template->InstanceTemplate();
	teapotTemplate->SetInternalFieldCount(1);

	colladaTemplate = collada_template->InstanceTemplate();
	colladaTemplate->SetInternalFieldCount(1);

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
