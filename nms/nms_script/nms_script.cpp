#include "nms_script.h"

Persistent<Context> nms_script::g_context;
Handle<ObjectTemplate> nms_script::lightTemplate;
Handle<ObjectTemplate> nms_script::colladaTemplate;
Handle<ObjectTemplate> nms_script::md2Template;
Handle<ObjectTemplate> nms_script::rootTemplate;
Handle<ObjectTemplate> nms_script::geometryNodeTemplate;

nms_script::nms_script()
{
	g_context = Context::New();
}

nms_script::~nms_script()
{

}

Handle<Script> nms_script::ReadAndCompileScript(const char * filename)
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

Persistent<Function> nms_script::GetFunctionHandle(const char * functionName)
{
	HandleScope handleScope;
	Persistent<Function> function;

	Handle<String> processName = String::New(functionName);
	Handle<Value> process_val = g_context->Global()->Get(processName);

	// bail if we couldn't find the function
	if( !process_val->IsFunction()) 
		return function;

	Handle<Function> process_fun = Handle<Function>::Cast(process_val);
	function = Persistent<Function>::New(process_fun);

	return function;

}

int nms_script::RunScripts()
{
	watcher.scanFiles();
	scripts = watcher.getScripts();
	for(int i = 0; i<scripts.size(); i++)
	{
		string scriptName = scripts[i];
		char* path = watcher.getPath();
		string fullPath = string(path) + scriptName;

		Run(fullPath.c_str());
	}
	return 0;
}

int nms_script::Run(const char *filename)
{
	HandleScope handleScope;

	// Create global Object template
	Handle<ObjectTemplate> global = ObjectTemplate::New();

	//Create function callbacks
	global->Set(String::New("Log"), FunctionTemplate::New(LogCallback));
	global->Set(String::New("setGravity"), FunctionTemplate::New(setGravity));
	//Create constructor callbacks
	global->Set(String::New("Light"), FunctionTemplate::New(constructLight));
	global->Set(String::New("Collada"), FunctionTemplate::New(constructCollada));
	global->Set(String::New("MD2"), FunctionTemplate::New(constructMD2));
	global->Set(String::New("GeometryNode"), FunctionTemplate::New(constructGeometryNode));
	global->Set(String::New("getRoot"), FunctionTemplate::New(getRoot));

	//Create Function templates
	Handle<FunctionTemplate> lightFuncTemplate = FunctionTemplate::New();
	Handle<FunctionTemplate> colladaFuncTemplate = FunctionTemplate::New();
	Handle<FunctionTemplate> md2FuncTemplate = FunctionTemplate::New();
	Handle<FunctionTemplate> rootFuncTemplate = FunctionTemplate::New();
	Handle<FunctionTemplate> geometryNodeFuncTemplate = FunctionTemplate::New();
	
	//Get instances of templates
	lightTemplate = lightFuncTemplate->InstanceTemplate();
	colladaTemplate = colladaFuncTemplate->InstanceTemplate();
	md2Template = md2FuncTemplate->InstanceTemplate();
	rootTemplate = rootFuncTemplate->InstanceTemplate();
	geometryNodeTemplate = geometryNodeFuncTemplate->InstanceTemplate();

	//Set internal field Counts
	lightTemplate->SetInternalFieldCount(1);
	colladaTemplate->SetInternalFieldCount(1);
	md2Template->SetInternalFieldCount(1);
	rootTemplate->SetInternalFieldCount(1);
	geometryNodeTemplate->SetInternalFieldCount(1);

	//Set accessors
	//lightTemplate.SetAccessor(String::New("setLightNumber"), GetPointX, SetPointX);

	//Set Methods
	lightTemplate->Set("setLightNumber", FunctionTemplate::New(setLightNumber));
	md2Template->Set("LoadModel", FunctionTemplate::New(LoadModelMD2));
	colladaTemplate->Set("LoadModel", FunctionTemplate::New(LoadModelCollada));
	rootTemplate->Set("addChild", FunctionTemplate::New(addChild));
	
	Handle<Context> context = Context::New(NULL,global);
	g_context = Persistent<Context>::New(context); // make the context global
	Context::Scope context_scope(g_context);

	Handle<Script> script = ReadAndCompileScript(filename);
	
	Persistent<Function> updateFunction = GetFunctionHandle("Update");
	printf("\n\n... Running Code ...\n\n");
	/*
	const int numArgs=0;
	Handle<Value> * args = NULL;
	Handle<Value> result = updateFunction->Call( g_context->Global(), numArgs, args);
	
	// Convert the result to an ASCII string and print it.
	String::AsciiValue ascii(result);
	printf("The Result is %s\n", *ascii);
	*/
	return 0;
}

Handle<Value> nms_script::getResult(Persistent<Function> function, Handle<Value> *args, const int numArgs)
{
	Handle<Value> result = function->Call(g_context->Global(), numArgs, args);
	return result;
}

// ##### GLOBAL FUNCTIONS #####

Handle<Value> nms_script::setGravity(const Arguments& args)
{ 
    int gravity = args[0]->Uint32Value();
	//Change the gravity
	engine.physics->setGravity(gravity);
    return args[0];
}

Handle<Value> nms_script::LogCallback(const Arguments &args)
{
	HandleScope handleScope;
	Local<Value> value =  args[0];
	String::AsciiValue ascii(value);
	//printf("The Result is %s\n", *ascii);
	LOG.write(*ascii, LOG_DEBUG);
	return value;
}

// ##### CONSTRUCTOR CALLBACKS #####
Handle<Value> nms_script::constructLight(const Arguments &args)
{
	// throw if called without 'new'
	if (!args.IsConstructCall()) 
		return ThrowException(String::New("Cannot call constructor as function"));
 
	//start a handle scope
	HandleScope handleScope;
 
	//generate a new point
	LightSource *light = new LightSource();
 
	//return the wrapped point
	return wrapLight(light);
}
 

Handle<Value> nms_script::constructCollada(const Arguments &args)
{
	// throw if called without 'new'
	if (!args.IsConstructCall()) 
		return ThrowException(String::New("Cannot call constructor as function"));
 
	//start a handle scope
	HandleScope handleScope;
 
	//generate a new point
	ColladaModel *collada = new ColladaModel();
 
	//return the wrapped point
	return wrapCollada(collada);
}

Handle<Value> nms_script::constructMD2(const Arguments &args)
{
	// throw if called without 'new'
	if (!args.IsConstructCall()) 
		return ThrowException(String::New("Cannot call constructor as function"));
 
	//start a handle scope
	HandleScope handleScope;
 
	//generate a new point
	MD2Model *MD2 = new MD2Model();
 
	//return the wrapped point
	return wrapMD2(MD2);
}

Handle<Value> nms_script::constructGeometryNode(const Arguments &args)
{
	// throw if called without 'new'
	if (!args.IsConstructCall()) 
		return ThrowException(String::New("Cannot call constructor as function"));
 
	//start a handle scope
	HandleScope handleScope;
 
	Local<External> MeshWrap = Local<External>::Cast(args[0]);
    void* Mesh = MeshWrap->Value();

	//generate a new point
	GeometryNode *node = new GeometryNode(static_cast<NMS_Mesh*>(Mesh), NULL);
 
	//return the wrapped point
	return wrapGeometryNode(node);
}

// ##### WRAPPERS #####

Handle<Object> nms_script::wrapLight(LightSource *lightToWrap) 
{
    HandleScope handleScope;
 
    //create a new instance
    Local<Object> instance = lightTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, External::New(lightToWrap));

    return handleScope.Close(instance);
}

Handle<Object> nms_script::wrapCollada(ColladaModel *colladaToWrap) 
{
    HandleScope handleScope;
 
    //create a new instance
    Local<Object> instance = colladaTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, External::New(colladaToWrap));

    return handleScope.Close(instance);
}

Handle<Object> nms_script::wrapMD2(MD2Model *MD2ToWrap) 
{
    HandleScope handleScope;
 
    //create a new instance
    Local<Object> instance = md2Template->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, External::New(MD2ToWrap));

    return handleScope.Close(instance);
}

Handle<Object> nms_script::wrapGeometryNode(GeometryNode *NodeToWrap) 
{
    HandleScope handleScope;
 
    //create a new instance
    Local<Object> instance = geometryNodeTemplate->NewInstance();
 
    //set internal field on instance
    instance->SetInternalField(0, External::New(NodeToWrap));

    return handleScope.Close(instance);
}

Handle<Value> nms_script::getRoot(const Arguments &args)
{
	HandleScope handleScope;

	SceneGraphNode* root = engine.getScene();

	Local<Object> instance = rootTemplate->NewInstance();

	instance->SetInternalField(0, External::New(root));
	
	return handleScope.Close(instance);
}

// ##### Methods #####

Handle<Value> nms_script::setLightNumber(const Arguments& args)
{
    Local<Object> self = args.Holder();
	int lightNumber = args[0]->Int32Value();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<LightSource*>(ptr)->setLightNumber(GL_LIGHT0+lightNumber);
	return Integer::New(lightNumber);
}

Handle<Value> nms_script::LoadModelMD2(const Arguments& args)
{
    Local<Object> self = args.Holder();

	//Get the argument convert it to ascii
	Local<Value> arg1 =  args[0];
	Local<Value> arg2 =  args[1];
	String::AsciiValue arg1_ascii(arg1);
	String::AsciiValue arg2_ascii(arg2);

    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<MD2Model*>(ptr)->LoadModel(*arg1_ascii, *arg2_ascii);
	return arg1;
}

Handle<Value> nms_script::LoadModelCollada(const Arguments& args)
{
    Local<Object> self = args.Holder();

	//Get the argument convert it to ascii
	Local<Value> arg1 =  args[0];
	Local<Value> arg2 =  args[1];
	String::AsciiValue arg1_ascii(arg1);
	String::AsciiValue arg2_ascii(arg2);

    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<MD2Model*>(ptr)->LoadModel(*arg1_ascii, *arg2_ascii);
	return arg1;
}

Handle<Value> nms_script::addChild(const Arguments& args)
{
	Local<Object> self = args.Holder();
	Local<External> wrap2 = Local<External>::Cast(args[0]);
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
	void* ptr2 = wrap2->Value();
    static_cast<SceneGraphNode*>(ptr)->addChild(static_cast<SceneGraphNode*>(ptr2));
	return Integer::New(1);
}