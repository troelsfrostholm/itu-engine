#include "NMS_SceneRenderer.h"


NMS_SceneRenderer::NMS_SceneRenderer() 
{
	this->physics = NULL;
	rendering = false; 
	sceneGraphRoot = NULL;
	vertexShaderFile = NULL;
	fragmentShaderFile = NULL;
	skybox = false;
	skyboxTexture = 0;
	stopped = false;
}

NMS_SceneRenderer::NMS_SceneRenderer(nms_physics *physics)
{
	this->physics = physics;
	rendering = false;
	skybox = false;
	skyboxTexture = 0;
	sceneGraphRoot = NULL;
	current_camera = NULL;
	vertexShaderFile = NULL;
	fragmentShaderFile = NULL;
	stopped = false;
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

	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, width, height); // Set the dimensions of the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glFrustum(    -0.5f,
                0.5f,
                -0.5f*(float)(height/width),
                0.5f*(float)(height/width),
                1.0f,
                500.0f);
		
	gluPerspective(60.0, (float)width/(float)height, 1.0, width);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Enable Light
	//glEnable(GL_TEXTURE_2D); //Initialize OpenGl and texture mapping
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glShadeModel(GL_SMOOTH);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	currentTime=1;
	return true;
}

void NMS_SceneRenderer::up()
{
	rendering=true;
	start();
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

void NMS_SceneRenderer::initShaders()
{
	NMS_SHADER_MANAGER->up();
	if(vertexShaderFile && fragmentShaderFile) {
		NMS_SHADER_MANAGER->loadShaders(vertexShaderFile, fragmentShaderFile);
	}
}

int NMS_SceneRenderer::run()
{
	initRendering();
	initShaders();
	renderingLoop();
	return 0;
}

void NMS_SceneRenderer::togglePause()
{
	stopped = !stopped;
}

int NMS_SceneRenderer::renderingLoop()
{
	while(rendering) {
		SINGLEFRAME_ALLOC->clear();
		NMS_EVENT_MANAGER.pollEvents();
		if(!stopped)
		{
			currentTime+=0.0008f;
			physics->simulatePhysics();
		}
		render();
		CalculateFrameRate();
	}
	return 0;
}

void NMS_SceneRenderer::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	SDL_LockMutex(sceneGraphGuard);
	Matrix m = Matrix();
	EmptySceneVisitor v = EmptySceneVisitor();
	if(skybox) renderSkyBox();
	current_camera->backtrack_to_root(&v, &m);
	sceneGraphRoot->traverse_df(this, &m);

	//debug drawing
	physics->getDynamicsWorld()->debugDrawWorld();

	SDL_UnlockMutex(sceneGraphGuard);
	SDL_GL_SwapBuffers();
}

void NMS_SceneRenderer::setScene(SceneGraphNode* scene)
{
	sceneGraphRoot = scene;
}

void NMS_SceneRenderer::setCurrentCamera(NMSCameraController* camera)
{
	current_camera = camera;
}

void NMS_SceneRenderer::setShaders(char * _vertexShaderFile, char * _fragmentShaderFile)
{
	vertexShaderFile = _vertexShaderFile;
	fragmentShaderFile = _fragmentShaderFile;
	if(rendering)
		NMS_SHADER_MANAGER->loadShaders(vertexShaderFile, fragmentShaderFile);
}

//Render meshes as they are traversed in the scene graph
void NMS_SceneRenderer::sg_before(Matrix transform, SceneGraphNode * node)
{
	GeometryNode * geomNode = (GeometryNode *) node;
	NMS_Mesh* model = geomNode->getModel();
	btRigidBody *b = geomNode->getCollisionBody();

	glLoadIdentity();
	Matrix t_transposed = ~transform;
	glMultMatrixf(t_transposed.getElements());
	if(b != NULL)
		applyPhysics(b);
	setWireframeModeGL(wireframe);
	(*model).setMaterialGL();
	(*model).render(currentTime);
}

void NMS_SceneRenderer::sg_after(Matrix transform, SceneGraphNode * node) {}

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

void NMS_SceneRenderer::setWireframeModeGL(bool state)
{
	if(state)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void NMS_SceneRenderer::setWireframeMode(bool mode)
{
	wireframe=mode;
}

void NMS_SceneRenderer::renderSkyBox()
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	//glColor3f(0, 0, 1);
	Matrix transform = !current_camera->returnViewMatrix();
	transform = transform.getRotation();
	glLoadIdentity();
	glMultMatrixf(transform.getElements());
	if(skyboxTexture == 0)
		skyboxTexture = NMS_ASSETMANAGER.LoadTexture("textures/wikimap.tga", "map");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);
	NMS_SHADER_MANAGER->setShaderAttribute("colorMap", 0);
	NMS_SHADER_MANAGER->setShaderAttribute("environmentmap", 0);
	NMS_SHADER_MANAGER->setShaderAttribute("lightingEnabled", 0);
	NMS_SHADER_MANAGER->enableTextures();
	

	float size = 100;
	float c = 0.00f; //correction inset for tex coords, so we are sure we are inside the tile
	glBegin(GL_QUADS);
		// Front Face
		//glColor3f(1, 0, 0);
		glTexCoord2f(0.25f, 0.333f); glVertex3f(-size, -size,  size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(0.5f, 0.333f); glVertex3f( size, -size,  size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(0.5f, 0.666f); glVertex3f( size,  size,  size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.25f, 0.666f); glVertex3f(-size,  size,  size);	// Top Left Of The Texture and Quad

		// Back Face
		//glColor3f(0, 1, 0);
		glTexCoord2f(1.0f, 0.333f); glVertex3f(-size, -size, -size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 0.666f); glVertex3f(-size,  size, -size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.75f, 0.666f); glVertex3f( size,  size, -size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.75f, 0.333f); glVertex3f( size, -size, -size);	// Bottom Left Of The Texture and Quad

		// Top Face
		//glColor3f(0, 0, 1);
		glTexCoord2f(0.25f+c, 1.0f); glVertex3f(-size,  size, -size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.25f+c, 0.666f); glVertex3f(-size,  size,  size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(0.5f, 0.666f); glVertex3f( size,  size,  size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(0.5f, 1.0f); glVertex3f( size,  size, -size);	// Top Right Of The Texture and Quad

		// Bottom Face
		//glColor3f(1, 1, 0);
		glTexCoord2f(0.5f, 0.333f); glVertex3f(-size, -size, -size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.25f, 0.333f); glVertex3f( size, -size, -size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.25f, 0.0f); glVertex3f( size, -size,  size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(0.5f, 0.0f); glVertex3f(-size, -size,  size);	// Bottom Right Of The Texture and Quad

		// Right face
		//glColor3f(0, 1, 1);
		glTexCoord2f(0.75f, 0.333f); glVertex3f( size, -size, -size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(0.75f, 0.666f); glVertex3f( size,  size, -size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.5f, 0.666f); glVertex3f( size,  size,  size);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.5f, 0.333f); glVertex3f( size, -size,  size);	// Bottom Left Of The Texture and Quad

		// Left Face
		//glColor3f(1, 0, 1);
		glTexCoord2f(0.0f, 0.333f); glVertex3f(-size, -size, -size);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(0.25f, 0.333f); glVertex3f(-size, -size,  size);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(0.25f, 0.666f-c); glVertex3f(-size,  size,  size);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 0.666f-c); glVertex3f(-size,  size, -size);	// Top Left Of The Texture and Quad
	glEnd();

	NMS_SHADER_MANAGER->setShaderAttribute("environmentmap", 1);
	NMS_SHADER_MANAGER->setShaderAttribute("lightingEnabled", 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void NMS_SceneRenderer::enableSkyBox(char * texture)
{
	skybox = true;
}

void NMS_SceneRenderer::disableSkyBox()
{
	skybox = false;
}