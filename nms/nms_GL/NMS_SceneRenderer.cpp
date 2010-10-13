#include "NMS_SceneRenderer.h"

NMS_SceneRenderer::NMS_SceneRenderer() 
{
	this->physics = NULL;
	rendering = false; 
	sceneGraphRoot = NULL;
}

NMS_SceneRenderer::NMS_SceneRenderer(nms_physics *physics) 
{
	this->physics = physics;
	rendering = false; 
	sceneGraphRoot = NULL;
	current_camera = NULL;
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
	

	//Enable Light
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	currentTime=1;
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
	inputGuard=NULL;
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
		currentTime+=0.0008f;
		NMS_EVENT.pollEvents();
		physics->simulatePhysics();
		physics->checkAllTriggers();
		render();
		CalculateFrameRate();
		//SDL_Delay(10);
	}
	return 0;
}

void NMS_SceneRenderer::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_LockMutex(sceneGraphGuard);
	Matrix m = Matrix();
	EmptySceneVisitor v = EmptySceneVisitor();
	current_camera->backtrack_to_root(&v, &m);
	sceneGraphRoot->traverse_df(this, &m);
	SDL_UnlockMutex(sceneGraphGuard);
	SDL_GL_SwapBuffers();
}

void NMS_SceneRenderer::setScene(SceneGraphNode* scene)
{
	sceneGraphRoot = scene;
}

void NMS_SceneRenderer::setCurrentCamera(CameraNode* camera)
{
	current_camera = camera;
}

//Render meshes as they are traversed in the scene graph
void NMS_SceneRenderer::sg_before(Matrix transform, NMS_Mesh* model, btRigidBody *b)
{
	glLoadIdentity();
	Matrix t_transposed = ~transform;
	glMultMatrixf(t_transposed.returnPointer());
	applyPhysics(b);
	(*model).render(currentTime);
}

void NMS_SceneRenderer::sg_after(Matrix transform, NMS_Mesh* model) {}

void NMS_SceneRenderer::CalculateFrameRate()
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

void NMS_SceneRenderer::applyPhysics(btRigidBody *b)
{
	btScalar matrix[16];
	btTransform trans;
	b->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(matrix);
	glMultMatrixf(matrix);
}
