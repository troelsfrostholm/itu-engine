#ifdef __EXP_NMS_GL
#    define SCENE_RENDERER_D __declspec(dllexport)
#else
#    define SCENE_RENDERER_D __declspec(dllimport)
#endif

#ifndef NMS_SCENE_RENDERER
#define NMS_SCENE_RENDERER

#include "SDL.h"
#include "NMS_Mutex.h"
#include "NMS_Thread.h"
#include "NMS_SceneGraph.h"

class SCENE_RENDERER_D NMS_SceneRenderer : public SceneGraphVisitor, public Thread
{
protected:
	SceneGraphNode* sceneGraphRoot;
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
	void setScene(SceneGraphNode* scene);
	void sg_before(Matrix transform, Mesh model, btRigidBody *b);
	void sg_after(Matrix transform, Mesh model);
	void NMS_SceneRenderer::applyPhysics(Matrix *m, btRigidBody *b);
};

#endif