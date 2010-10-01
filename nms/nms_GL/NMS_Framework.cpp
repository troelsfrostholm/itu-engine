#include "NMS_Framework.h"


NMS_Framework::NMS_Framework(){};

bool NMS_Framework::NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	//Instantiate sub-systems
	sceneRenderer = NMS_SceneRenderer();
	
	//Create scene-graph
	sceneGraphRoot = new TransformationNode(Matrix());
	sceneGraphGuard = SDL_CreateMutex();
	sceneRenderer.setScene(sceneGraphRoot, sceneGraphGuard);

	//set callback for quitting
	NMS_EVENT.onQuit(this, &NMS_Framework::NMSQuit);

	//try to initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
		{ return false; } 
	
	if (fullscreen) {
		flags = SDL_OPENGL | SDL_FULLSCREEN;
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}
	else
	{
		flags = SDL_OPENGL;
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}
	SDL_WM_SetCaption(windowTitle,NULL);  //Set the name of the window
	SDL_SetVideoMode(width, height, bpp, flags); //Set the window mode

	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, width, height); // Set the dimensions of the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D); //Initialize OpenGl and texture mapping
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glFrustum(    -0.5f,
                0.5f,
                -0.5f*(float)(height/width),
                0.5f*(float)(height/width),
                1.0f,
                500.0f);
		
	gluPerspective(60.0, (float)width/(float)height, 1.0, width);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

void NMS_Framework::enableWireframe(bool reply)
{
	if(reply)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}