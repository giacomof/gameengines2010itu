#include "scriptHandler.h"


ScriptHandler::ScriptHandler(void)
{
}


ScriptHandler::~ScriptHandler(void)
{
}

Handle<Script> ScriptHandler::readAndCompileScript(const char * filename)
{

	Handle<Script> script = Handle<Script>();
	
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