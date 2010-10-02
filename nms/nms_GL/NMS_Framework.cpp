#include "NMS_Framework.h"

NMS_Framework::NMS_Framework(){};

bool NMS_Framework::NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	//Initialize mutexes
	initMutexes();

	//Instantiate sub-systems
	sceneRenderer = NMS_SceneRenderer();
	
	//Create scene-graph
	sceneGraphRoot = new TransformationNode(Matrix());
	sceneRenderer.setScene(sceneGraphRoot);

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
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;
	static char strCaption[80] = {0};
	static size_t strCaptionSize = 80*sizeof(char);
	float currentTime = SDL_GetTicks() * 0.001f;
	++framesPerSecond;
	if( currentTime - lastTime > 1.0f )
	{
		lastTime = currentTime;
		sprintf_s(strCaption,strCaptionSize,"Frames per Second: %d",int(framesPerSecond));
		SDL_WM_SetCaption(strCaption,NULL);
		framesPerSecond = 0;
	}
}

void NMS_Framework::run()
{
	sceneRenderer.up();
	while(running)
	{
		NMS_EVENT.processEvents();
	}
	sceneRenderer.down();
	NMS_Framework::cleanup();
}

void NMS_Framework::cleanup()
{
	//Be sure to remove all the assets we have loaded!
	NMS_ASSETMANAGER.FreeAll();
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

void NMS_Framework::enableWireframe(bool reply)
{
	if(reply)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}