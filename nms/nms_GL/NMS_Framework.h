#ifdef __EXP_NMS_GL
#    define FRAMEWORK_D __declspec(dllexport)
#else
#    define FRAMEWORK_D __declspec(dllimport)
#endif

#ifndef NMS_Framework_H__
#define NMS_Framework_H__

#define NOMINMAX

#include "NMS_SceneRenderer.h"
#include "Matrix.h"
#include "Trig.h"
#include "NMS_AssetManager.h"
#include "NMS_EventManager.h"
#include "NMS_Camera.h"
#include "NMS_LightSystem.h"
#include "SDL_MemberThread.h"
#include "NMS_Mutex.h"
#include "nms_physics.h"
#include "nms_script.h"
#include "NMS_StaticAllocator.h"
#include "NMS_LogFunctions.h"

class FRAMEWORK_D NMS_Framework
{
protected:
	int flags;
	NMS_SceneRenderer sceneRenderer;
	SceneGraphNode* sceneGraphRoot;
	nms_script* scriptManager;
	bool bDebugEnable;
	
public:
	nms_physics* physics;
	bool running;
	bool rendering;
	bool stopped;
	NMSCameraFPS camera;
	NMS_Framework();
	bool NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen);
	void NMSQuit(int i);
	void run();
	void render();
	int renderingLoop();
	void processEvents();
	void enableWireframe(bool reply);
	static void NMSLoadTexture(char* fileName,int id);
	void CalculateFrameRate();
	void setDebugMode(bool mode);
	void cleanup();
	SceneGraphNode* getScene();
	NMS_SceneRenderer* getRenderer();
	static void logMemoryUsage();
	void togglePause(int none);
};
#endif;

