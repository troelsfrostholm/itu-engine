#ifdef __EXP_NMS_GL
#    define FRAMEWORK_D __declspec(dllexport)
#else
#    define FRAMEWORK_D __declspec(dllimport)
#endif

#ifndef NMS_Framework_H__
#define NMS_Framework_H__

#define NOMINMAX

#include "SDL.h"
#include "SDL_opengl.h" 
#include "Matrix.h"
#include "Trig.h"
#include "NMS_AssetManager.h"
#include "NMS_EventManager.h"
#include "NMS_Camera.h"
#include "NMS_SceneRenderer.h"
#include "NMS_LightSystem.h"
#include "SDL_MemberThread.h"
#include "NMS_Mutex.h"
#include "nms_physics.h"
#include "NMS_StaticAllocator.h"

class FRAMEWORK_D NMS_Framework
{
protected:
	int flags;
	NMS_SceneRenderer sceneRenderer;
	SceneGraphNode* sceneGraphRoot;
	
	
public:
	nms_physics* physics;
	bool running;
	bool rendering;
	NMSCameraFPS camera;
	NMS_LightSystem light;
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
	void cleanup();
	SceneGraphNode* getScene();
	NMS_SceneRenderer* getRenderer();
};
#endif;

