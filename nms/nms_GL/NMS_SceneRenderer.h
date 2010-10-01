#ifdef __EXP_NMS_GL
#    define SCENE_RENDERER_D __declspec(dllexport)
#else
#    define SCENE_RENDERER_D __declspec(dllimport)
#endif

#ifndef NMS_SCENE_RENDERER
#define NMS_SCENE_RENDERER

#include "SDL.h"
#include "NMS_Thread.h"
#include "NMS_SceneGraph.h"

class SCENE_RENDERER_D NMS_SceneRenderer : public SceneGraphVisitor, public Thread
{
protected:
	SceneGraphNode* sceneGraphRoot;
	SDL_mutex* sceneGraphGuard;
//	SDL_Thread *renderThread;
	bool rendering;

public:
	NMS_SceneRenderer();
	bool initRendering();
	void up();
	void down();
	int run();
	int renderingLoop();
	void render();
	void setScene(SceneGraphNode* scene, SDL_mutex* sceneGuard);
	void sg_before(Matrix transform, Mesh model);
	void sg_after(Matrix transform, Mesh model);
};

#endif