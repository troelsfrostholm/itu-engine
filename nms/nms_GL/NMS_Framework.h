#ifndef NMS_Framework_H__
#define NMS_Framework_H__

#include "SDL.h" 
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Trig.h"
#include "NMS_AssetManager.h"
#include "NMS_Event.h"
#include "NMS_Camera.h"
#include "NMS_SceneRenderer.h"
#include "SDL_MemberThread.h"

class __declspec(dllexport) NMS_Framework
{
protected:
	int flags;
	NMS_SceneRenderer sceneRenderer;
	SceneGraphNode* sceneGraphRoot;
	SDL_mutex* sceneGraphGuard;
	
public:
	bool running;
	bool rendering;
	NMSCameraFPS camera;
	NMS_Framework();
	bool NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen);
	void NMSQuit(int i);
	void run();
	void render();
	int renderingLoop();
	void processEvents();
	static void NMSLoadTexture(char* fileName,int id);
	void CalculateFrameRate();
	void cleanup();
	SceneGraphNode* getScene();
};
#endif;

