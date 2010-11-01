/****************************************************************************************
DLL EXPORTING SAFE
*****************************************************************************************/
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
#include "NMS_EventManager.h"
#include "NMS_Mesh.h"
#include "nms_physics.h"

class SCENE_RENDERER_D NMS_SceneRenderer : public SceneGraphVisitor, public Thread
{
protected:
	SceneGraphNode* sceneGraphRoot;
	CameraNode* current_camera;
	nms_physics *physics;
	//SDL_Thread *renderThread;
	bool rendering;
	float currentTime;
	bool wireframe;

public:
	NMS_SceneRenderer();
	NMS_SceneRenderer(nms_physics *physics);
	bool initRendering();
	void up();
	void down();
	int run();
	int renderingLoop();
	void render();
	void setScene(SceneGraphNode* scene);
	void setCurrentCamera(CameraNode* camera);
	void sg_before(Matrix transform, SceneGraphNode * node);
	void sg_after(Matrix transform, SceneGraphNode * node);
	void CalculateFrameRate();
	void applyPhysics(btRigidBody *b);
	void setWireframeMode(bool mode);
protected:
	void setWireframeModeGL(bool mode);
};

#endif