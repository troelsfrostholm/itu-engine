#include "NMS_SceneRenderer.h"

NMS_SceneRenderer::NMS_SceneRenderer() { 
	rendering = false; 
	sceneGraphRoot = NULL;
	sceneGraphGuard = NULL;
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
	renderingLoop();
	return 0;
}

int NMS_SceneRenderer::renderingLoop()
{
	while(rendering) {
		render();
	}

	return 0;
}

void NMS_SceneRenderer::render()
{
	//SDL_LockMutex(sceneGraphGuard);
	sceneGraphRoot->traverse_df(this);
	//SDL_UnlockMutex(sceneGraphGuard);
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
	glMultMatrixf(transform.returnPointer());
	model.render();

}

void NMS_SceneRenderer::sg_after(Matrix transform, Mesh model) {}