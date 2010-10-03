#ifdef __EXP_NMS_GL
#    define FRAMEWORK_D __declspec(dllexport)
#else
#    define FRAMEWORK_D __declspec(dllimport)
#endif

#ifndef NMS_Framework_H__
#define NMS_Framework_H__

#include "SDL.h" 
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include "Matrix.h"
#include "Trig.h"
#include "NMS_AssetManager.h"
#include "NMS_Event.h"
#include "NMS_Camera.h"
#include "NMS_SceneRenderer.h"
#include "NMS_LightSystem.h"
#include "SDL_MemberThread.h"
#include "NMS_Mutex.h"

class FRAMEWORK_D NMS_Framework
{
protected:
	int flags;
	NMS_SceneRenderer sceneRenderer;
	SceneGraphNode* sceneGraphRoot;
	
	
public:
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
};
#endif;

