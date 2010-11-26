#include "scriptHandler.h"

Persistent<Context> ScriptHandler::g_context;

ScriptHandler::ScriptHandler(void)
{

}


ScriptHandler::~ScriptHandler(void)
{

}

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

Handle<Value> ScriptHandler::LogCallback(const Arguments &args)
{
	HandleScope handleScope;
	int numArgs = args.Length();
	Local<Value> value =  args[0];
	String::AsciiValue ascii(value);
	printf("The Result is %s\n", *ascii);
	return value;

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

void ScriptHandler::runScript(void) 
{
	HandleScope handleScope;

	// Create a template for the global object and set the
	// built-in global functions.
	Local<ObjectTemplate> globals = ObjectTemplate::New();
	globals->Set( String::New("Log"), FunctionTemplate::New( LogCallback ) );

	// Each processor gets its own context so different processors
	// do not affect each other.
	Handle<Context> context = Context::New( NULL, globals );

	g_context = Persistent<Context>::New(context); // make the context global
	Context::Scope scope(g_context);

	Persistent<Function> updateFunction = GetFunctionHandle("test.js","Update");
	
	const int numArgs=0;
	Handle<Value> * args = NULL;
	Handle<Value> result = updateFunction->Call( g_context->Global(), numArgs, args);
	

	// Convert the result to an ASCII string and print it.
	String::AsciiValue ascii(result);
	printf("The Result is %s\n", *ascii);
}