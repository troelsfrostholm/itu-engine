#ifndef NMS_SCENE_RENDERER
#define NMS_SCENE_RENDERER

#include "SDL.h"
#include "SDL_MemberThread.h"
#include "NMS_SceneGraph.h"

class __declspec(dllexport) NMS_SceneRenderer : public SceneGraphVisitor
{
protected:
	SceneGraphNode* sceneGraphRoot;
	SDL_mutex* sceneGraphGuard;
	SDL_Thread *renderThread;
	bool rendering;

public:
	NMS_SceneRenderer();
	void up();
	void down();
	int renderingLoop();
	void render();
	void setScene(SceneGraphNode* scene, SDL_mutex* sceneGuard);
	void sg_before(Matrix transform, Mesh model);
	void sg_after(Matrix transform, Mesh model);
};

#endif