#include "SDL.h"
#include <cmath>
#include <v8.h>

using namespace v8;

Persistent<Context> g_context;

Handle<Script> ReadAndCompileScript(const char * filename)
{
	Handle<Script> script;
	// Create a string containing the JavaScript source code.
	FILE * sourceFile = fopen(filename,"rt");
	if( sourceFile == NULL )
	{
		printf("aborting!! Could not open file!");
		return script;
	}

	char buffer[20480];
	int bytesRead = fread( buffer, 1, 20480, sourceFile);
	if( bytesRead == -1 )
	{
		printf("aborting!! Could not read from file!");
		fclose(sourceFile);
		return script;
	}
	buffer[bytesRead]='\0';

	printf("Script, length=%d, source = \n%s\n",strlen(buffer),buffer);
	Handle<String> source = String::New(buffer);
	fclose(sourceFile);

	// Compile the source code.
	script = Script::Compile(source);
	script->Run();
	return script;
}

Handle<Value> LogCallback(const Arguments &args)
{
	HandleScope handleScope;
	int numArgs = args.Length();
	Local<Value> value =  args[0];
	String::AsciiValue ascii(value);
	printf("The Result is %s\n", *ascii);
	return value;
}

Persistent<Function> GetFunctionHandle(const char * filename, const char * functionName)
{
	HandleScope handle_scope;
	Persistent<Function> function;


	Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();


	Handle<Script> script = ReadAndCompileScript(filename);

	Handle<String> processName = String::New(functionName);
	Handle<Value> process_val = g_context->Global()->Get(processName);

	// bail if we couldn't find the function
	if( !process_val->IsFunction()) 
		return function;

	Handle<Function> process_fun = Handle<Function>::Cast(process_val);
	function = Persistent<Function>::New(process_fun);

	return function;

}

int main(int argc, char* argv[])
{
	HandleScope handleScope;

	Local<FunctionTemplate> logFunction = FunctionTemplate::New(LogCallback);
	Local<ObjectTemplate> globals = ObjectTemplate::New();
	globals->Set(String::New("Log"),logFunction);

	Handle<Context> context = Context::New(NULL,globals);
	g_context = Persistent<Context>::New(context); // make the context global
	Context::Scope scope(g_context);
	
	Persistent<Function> updateFunction = GetFunctionHandle("test.js","Update");
	printf("\n\n... Running Code ...\n\n");
	const int numArgs=0;
	Handle<Value> * args = NULL;
	Handle<Value> result = updateFunction->Call( g_context->Global(), numArgs, args);
	
	// Convert the result to an ASCII string and print it.
	String::AsciiValue ascii(result);
	printf("The Result is %s\n", *ascii);

	return 0;
}