#ifdef __EXP_NMS_GL
#    define SCRIPT_D __declspec(dllexport)
#else
#    define SCRIPT_D __declspec(dllimport)
#endif

#ifndef NMS_SCRIPT_H__
#define NMS_SCRIPT_H__

#include <vector>
#include <cmath>
#include "string.h"
#include "NMS_LogFunctions.h"
#include "nms_physics.h"
#include "MD2Model.h"
#include "ColladaModel.h"
#include "NMS_LightSystem.h"
#include "nms_watcher.h"
#include "NMS_SceneGraph.h"
#include <v8.h>

using namespace v8;

class SCRIPT_D nms_script
{
	private:
		std::vector<std::string> scripts;
		nms_watcher watcher;
		SceneGraphNode* root;

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
#endif;
