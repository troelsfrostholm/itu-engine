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
	SDL_GL_SwapBuffers();
}