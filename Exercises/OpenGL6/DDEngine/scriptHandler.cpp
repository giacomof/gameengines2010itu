#include "scriptHandler.h"

Persistent<Context> ScriptHandler::g_context;

ScriptHandler::ScriptHandler(void)
{
	//runScript("test.js", "Update");
	//WindowManager window = WindowManager::getInstance();
	//window.createWindow(320, 240, 32);
	//window.resizeWindow(320, 240);
}


ScriptHandler::~ScriptHandler(void)
{

}

// ******************************************************************
// ******************** IMPLEMENTED CALLBACKS ***********************
// ******************************************************************

// ************* CONSTRUCTORS ****************



// ************* LOG CALLBACKS ***************

Handle<Value> ScriptHandler::LogCallback(const Arguments &args)
{
	HandleScope handleScope;
	int numArgs = args.Length();
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


// ******************************************************************
// ********************** BASIC SCRIPT METHODS **********************
// ******************************************************************

Handle<Script> ScriptHandler::readAndCompileScript(const char * filename)
{

	Handle<Script> script;
	
	FILE * sourceFile = fopen( filename, "rt" );
	
	if( sourceFile == NULL )
	{
		cout << "ERROR FINDING THE FILE" << endl;
		return script;
	}

	char buffer[20480];
	int bytesRead = fread( buffer, 1, 20480, sourceFile);
	
	if( bytesRead == -1 )
	{
		cout << "ERROR READING THE FILE" << endl;
		fclose(sourceFile);
		return script;

	}
	
	buffer[bytesRead]='\0';

	printf("Script, length=%d, source = \n%s\n", strlen(buffer), buffer);
	
	Handle<String> source = String::New(buffer);
	fclose(sourceFile);

	// Compile the source code.
	script = Script::Compile(source);
	script->Run();
	return script;
}

Persistent<Function> ScriptHandler::GetFunctionHandle(const char * filename, const char * functionName)
{

	HandleScope handle_scope;
	Persistent<Function> function;
	
	Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();

	Handle<Script> script = readAndCompileScript(filename);

	Handle<String> processName = String::New(functionName);
	Handle<Value> process_val = g_context->Global()->Get(processName);

	// bail if we couldn't find the function
	if( !process_val->IsFunction() ) 
		return function;

	Handle<Function> process_fun = Handle<Function>::Cast(process_val);
	function = Persistent<Function>::New(process_fun);

	return function;

}

void ScriptHandler::runScript(const char * filename, const char * function) 
{
	HandleScope handleScope;

	// Create a template for the global object and set the built-in global functions.
	Local<ObjectTemplate> globals = ObjectTemplate::New();
	
	globals->Set( String::New("Log"), FunctionTemplate::New( LogCallback ) );
	globals->Set( String::New("ClearLog"), FunctionTemplate::New( clearLogCallback ) );
	//globals->Set( String::New("String"), FunctionTemplate::New( constructString ) );

	// Each processor gets its own context so different processors do not affect each other.
	Handle<Context> context = Context::New( NULL, globals );
	// make the context global
	g_context = Persistent<Context>::New(context); 
	Context::Scope scope(g_context);

	Persistent<Function> updateFunction = GetFunctionHandle(filename, function);
	
	const int numArgs = 0;
	Handle<Value> * args = NULL;
	Handle<Value> result = updateFunction->Call( g_context->Global(), numArgs, args);

}

Handle<Context> ScriptHandler::getContext(void) 
{
	return g_context;
}


// this template method returns a scriptHandler pointer starting from an External
template <typename T>
    static T * externalToClassPtr(Local<Value> data)
    {
        if(data.IsEmpty())
            cout<<"Data empty"<<endl;
        else if(!data->IsExternal())
            cout<<"Data not external"<<endl;
        else
            return static_cast<T *>(External::Unwrap(data));
                
        //If function gets here, one of the checks above failed
        return NULL;
    }

// Wrap a callback function into a FunctionTemplate, providing the "this"
// pointer to the callback when v8 calls the callback func
Local<FunctionTemplate> ScriptHandler::makeStaticCallableFunc(InvocationCallback func)
{
    HandleScope scope;
    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(func, classPtrToExternal());
    return scope.Close(funcTemplate);
}

Local<External> ScriptHandler::classPtrToExternal()
{
    HandleScope scope;
    return scope.Close(External::New(reinterpret_cast<void *>(this)));
}