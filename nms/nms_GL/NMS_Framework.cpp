#include "NMS_Framework.h"

NMS_Framework::NMS_Framework(){};

bool NMS_Framework::NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	//Initialize mutexes
	initMutexes();

	//Instantiate sub-systems
	sceneRenderer = NMS_SceneRenderer();
	
	//Create scene-graph
	sceneGraphRoot = new CameraNode();
	sceneRenderer.setScene(sceneGraphRoot);
	sceneRenderer.setCurrentCamera((CameraNode*)sceneGraphRoot);

	//set callback for quitting
	NMS_EVENT.onQuit(this, &NMS_Framework::NMSQuit);

	camera=NMSCameraFPS::NMSCameraFPS();
	camera.setPos(Vector(0,0,-5.0f));
	camera.setSpeed(0);
	camera.setSlideSpeed(0);
	light=NMS_LightSystem::NMS_LightSystem();
	running=true;
	return true;
}

void NMS_Framework::NMSQuit(int i)
{
	running=false;
}

void NMS_Framework::CalculateFrameRate()
{
	sceneRenderer.CalculateFrameRate();
}

void NMS_Framework::run()
{
	sceneRenderer.up();
	while(running)
	{
		SDL_LockMutex(sceneGraphGuard);
		NMS_EVENT.processEvents();
		SDL_UnlockMutex(sceneGraphGuard);
		SDL_Delay(10);
	}
	sceneRenderer.down();
	NMS_Framework::cleanup();
}

void NMS_Framework::cleanup()
{
	//Be sure to remove all the textures we have loaded from the memory!
	NMS_TEXTUREMANAGER.FreeAll();
	delete(sceneGraphRoot);
	SDL_DestroyMutex(sceneGraphGuard);
	sceneGraphRoot = NULL;
	SDL_Quit();
}

void NMS_Framework::render()
{
	sceneRenderer.render();
}

SceneGraphNode* NMS_Framework::getScene()
{
	return sceneGraphRoot;
}

NMS_SceneRenderer* NMS_Framework::getRenderer()
{
	return &sceneRenderer;
}

void NMS_Framework::enableWireframe(bool reply)
{
	if(reply)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}