#include "NMS_Framework.h"

NMS_Framework::NMS_Framework(){};

bool NMS_Framework::NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	//Clear memory usage array
	//Static array in nms_utilities/NMS_MemoryAllocation.h
	//Requires a refactoring!!
	memset(&CurrentMemoryUsage, 0, sizeof(int)*MEM_NUM_CATEGORIES);

	//Initialize mutexes
	initMutexes();

	physics = new(STATIC_ALLOC, MEM_PERSISTENT) nms_physics();

	//Instantiate sub-systems
	sceneRenderer = NMS_SceneRenderer(physics);
	
	//Create scene-graph
	sceneGraphRoot = new(STATIC_ALLOC, MEM_PERSISTENT) CameraNode();
	sceneRenderer.setScene(sceneGraphRoot);
	sceneRenderer.setCurrentCamera((NMSCameraController*)sceneGraphRoot);

	//set callback for quitting
	NMS_EVENT_MANAGER.onQuit(this, &NMS_Framework::NMSQuit);
	NMS_EVENT_MANAGER.onPause(this, &NMS_Framework::togglePause);

	camera=NMSCameraFPS::NMSCameraFPS();
	camera.setPos(Vector(0,0,-5.0f));
	camera.setSpeed(0);
	camera.setSlideSpeed(0);

	scriptManager = new nms_script(sceneGraphRoot);
	stopped = false;
	
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
		NMS_EVENT_MANAGER.processEvents();
		SDL_UnlockMutex(sceneGraphGuard);
		SDL_Delay(10);
	}
	sceneRenderer.down();
	NMS_Framework::logMemoryUsage();
	NMS_Framework::cleanup();
}

void NMS_Framework::cleanup()
{
	//Be sure to remove all the assets we have loaded!
	NMS_ASSETMANAGER.FreeAll();
	SDL_DestroyMutex(sceneGraphGuard);
	sceneGraphRoot = NULL;
	delete LEVEL_ALLOC;
	delete STATIC_ALLOC;
	SDL_Quit();
}

void NMS_Framework::togglePause(int none)
{
	stopped = !stopped;
	if(!stopped)
	{
		scriptManager->RunScripts();
	}
	sceneRenderer.togglePause();

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

void NMS_Framework::setDebugMode(bool mode)
{
	bDebugEnable=mode;
}

void NMS_Framework::logMemoryUsage()
{
	LOG.write("Memory usage at end of program : ", LOG_RUN);
	char output[50];
	sprintf(output, "\r\n    Persistent       : %d", CurrentMemoryUsage[MEM_PERSISTENT]);
	LOG.write(output, LOG_RUN);
	sprintf(output, "\r\n    Level            : %d", CurrentMemoryUsage[MEM_LEVEL]);
	LOG.write(output, LOG_RUN);
	sprintf(output, "\r\n    Matrix           : %d", CurrentMemoryUsage[MEM_MATRIX]);
	LOG.write(output, LOG_RUN);
	sprintf(output, "\r\n    Temporary vars   : %d", CurrentMemoryUsage[MEM_TEMP]);
	LOG.write(output, LOG_RUN);
}