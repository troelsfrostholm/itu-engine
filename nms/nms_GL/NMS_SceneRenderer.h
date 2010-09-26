#ifndef NMS_SCENE_RENDERER
#define NMS_SCENE_RENDERER

#include "SDL.h"
#include "SDL_MemberThread.h"
#include "NMS_SceneGraph.h"

class NMS_SceneRenderer : public SceneGraphVisitor
{
protected:
	bool rendering;
	SDL_Thread *renderThread;

public:
	NMS_SceneRenderer();
	void up();
	void down();
	int renderingLoop();
	void render();
};

#endif