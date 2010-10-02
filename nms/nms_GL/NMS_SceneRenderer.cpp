#include "NMS_SceneRenderer.h"
#include "NMS_Event.h"

NMS_SceneRenderer::NMS_SceneRenderer() { 
	rendering = false; 
	sceneGraphRoot = NULL;
	sceneGraphGuard = NULL;
}

bool NMS_SceneRenderer::initRendering()
{
	bool fullscreen = false;
	int width = 600;
	int height = 400;
	int flags;
	char* windowTitle = "Demo 2";
	bool bpp = false;

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

	
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
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
	/*glFrustum(    -0.5f,
                0.5f,
                -0.5f*(float)(height/width),
                0.5f*(float)(height/width),
                1.0f,
                500.0f);*/
		
	gluPerspective(60.0, (float)width/(float)height, 1.0, width);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return true;
}

void NMS_SceneRenderer::up()
{
	rendering=true;
	start();
	//renderThread = SDL_CreateMemberThread(this, &NMS_SceneRenderer::renderingLoop);
	//std::bind1st(std::mem_fun(&NMS_SceneRenderer::renderingLoop), this)
	//renderThread = SDL_CreateThread(, NULL);
}

void NMS_SceneRenderer::down()
{
	rendering=false;
	SDL_Thread* me = getThread();
	if(me)	
		SDL_WaitThread(me, NULL);
	sceneGraphRoot = NULL;
	sceneGraphGuard = NULL;
}

int NMS_SceneRenderer::run()
{
	initRendering();
	renderingLoop();
	return 0;
}

int NMS_SceneRenderer::renderingLoop()
{
	while(rendering) {
		NMS_EVENT.processEvents();
		render();
	}

	return 0;
}

void NMS_SceneRenderer::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_LockMutex(sceneGraphGuard);
	sceneGraphRoot->traverse_df(this);
	SDL_UnlockMutex(sceneGraphGuard);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//Mesh m = Mesh();
	//m.render();
	SDL_GL_SwapBuffers();
}

void NMS_SceneRenderer::setScene(SceneGraphNode* scene, SDL_mutex* sceneGuard)
{
	sceneGraphRoot = scene;
	sceneGraphGuard = sceneGuard;
}

//Render meshes as they are traversed in the scene graph
void NMS_SceneRenderer::sg_before(Matrix transform, Mesh model)
{
	glLoadIdentity();
	//glMultMatrixf(transform.returnPointer());
	//applyPhysics()
	model.render();

}

void NMS_SceneRenderer::sg_after(Matrix transform, Mesh model) {}