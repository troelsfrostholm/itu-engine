#include "NMS_SceneRenderer.h"

NMS_SceneRenderer::NMS_SceneRenderer() { }

void NMS_SceneRenderer::up()
{
	rendering=true;
	renderThread = SDL_CreateMemberThread(this, &NMS_SceneRenderer::renderingLoop);
}

void NMS_SceneRenderer::down()
{
	rendering=false;
	SDL_WaitThread(renderThread, NULL);
	sceneGraphRoot = NULL;
	sceneGraphGuard = NULL;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_LockMutex(sceneGraphGuard);
	sceneGraphRoot->traverse_df(this);
	SDL_UnlockMutex(sceneGraphGuard);
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
	glMultMatrixf(transform.returnPointer());
	model.render();

}

void NMS_SceneRenderer::sg_after(Matrix transform, Mesh model) {}