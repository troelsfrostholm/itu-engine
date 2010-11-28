#pragma unmanaged
#include <vector>
#include <cmath>
#include "string.h"
#include "NMS_LogFunctions.h"
#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "nms_physics.h"
#include "MD2Model.h"
#include "ColladaModel.h"
#include "NMS_Mutex.h"
#include "NMS_LightSystem.h"
#include "NMS_Audio.h"
#include "NMS_Camera.h"
#include "nms_watcher.h"
#pragma managed
#include <v8.h>

using namespace v8;

class nms_script
{
	private:
		std::vector<std::string> scripts;
		nms_watcher watcher;
	public:
		static Persistent<Context> g_context;

		// Object templates
		static Handle<ObjectTemplate> lightTemplate;
		static Handle<ObjectTemplate> colladaTemplate;
		static Handle<ObjectTemplate> md2Template;
		static Handle<ObjectTemplate> rootTemplate;
		static Handle<ObjectTemplate> geometryNodeTemplate;

		nms_script();
		~nms_script();
		int RunScripts();
		int Run(const char *filename);
		Handle<Script> ReadAndCompileScript(const char * filename);
		Persistent<Function> GetFunctionHandle(const char * functionName);	
		Handle<Value> getResult(Persistent<Function> function, Handle<Value> *args, const int numArgs);
		
		//Function callbacks
		static Handle<Value> setGravity(const Arguments& args);
		static Handle<Value> LogCallback(const Arguments &args);

		//Constructor callbacks
		static Handle<Value> constructCollada(const Arguments &args);
		static Handle<Value> constructLight(const Arguments &args);
		static Handle<Value> constructMD2(const Arguments &args);
		static Handle<Value> constructGeometryNode(const Arguments &args);

		//Wrappers
		static Handle<Object> wrapCollada(ColladaModel *colladaToWrap); 
		static Handle<Object> wrapLight(LightSource *lightToWrap);
		static Handle<Object> wrapMD2(MD2Model *MD2ToWrap);
		static Handle<Object> wrapGeometryNode(GeometryNode *NodeToWrap);
		static Handle<Value> getRoot(const Arguments &args);

		//Methods
		static Handle<Value> setLightNumber(const Arguments& args);
		static Handle<Value> LoadModelMD2(const Arguments& args);
		static Handle<Value> LoadModelCollada(const Arguments& args);
		static Handle<Value> addChild(const Arguments& args);
		
};