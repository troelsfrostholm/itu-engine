#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "nms_physics.h"
#include "MD2Model.h"
#include "ColladaModel.h"
#include <cmath>
#include "NMS_Mutex.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "NMS_LightSystem.h"
#include "NMS_Audio.h"
#include "NMS_Camera.h"

#define WIDTH  640
#define HEIGHT  480
//NMS_Framework engine = NMS_Framework();

TransformationNode traNode;
TransformationNode traNode2;
TransformationNode traNode3;
TransformationNode rotNode;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;
TransformationNode camTNode;
btRigidBody* fallRigidBody;
btRigidBody* fallRigidBody2;
btRigidBody* fallRigidBody3;
btRigidBody* fallRigidBody4;
GeometryNode geom;
GeometryNode satelite;
NMSCameraFPS cam;


void mouseMoved(int MouseX, int MouseY)
{
	Vector rot=cam.getRotation();
	//SDL_GetRelativeMouseState(&MouseX,&MouseY);
	//Pitch rotation, with the mouse is up and down
	rot[NMS_X]+=(MouseY)*0.03f;
	//Yaw rotation, with the mouse you can do it using the left right position
	rot[NMS_Y]+=(MouseX)*0.03f;	
	cam.setRotation(rot[NMS_X],rot[NMS_Y],rot[NMS_Z]);
}

void keyReleased(SDLKey key)
{
	switch( key ) {
		case SDLK_a:
			 cam.setSlideSpeed(0);
			 break;
		case SDLK_d:
			 cam.setSlideSpeed(0);
			 break;
		case SDLK_w:
			 cam.setSpeed(0);
			 break;
		case SDLK_s:
			 cam.setSpeed(0);
			 break;
	}
}


void keyPressed(SDLKey key)
{
	btVector3 vec;
	switch( key ) {
		 case SDLK_a:
			 cam.setSlideSpeed(+1.f);
				   break;
		 case SDLK_d:
			 cam.setSlideSpeed(-1.f);
				   break;
		 case SDLK_w:
			 cam.setSpeed(+2.f);
				   break;
		 case SDLK_s:
			 cam.setSpeed(-2.f);
				   break;

		 case SDLK_1:
			 engine.getRenderer()->setShaders("shaders\\fixedfunction.vertex", "shaders\\fixedfunction.fragment");
			       break;

		 case SDLK_q:
			 if(engine.physics->debugDrawer.getDebugMode() == 0)
			 {
				 engine.physics->debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
			 }
			 else
			 {
				 engine.physics->debugDrawer.setDebugMode(btIDebugDraw::DBG_NoDebug);
			 }
			 break;
	}
}

void idle( int i )
{
	cam.UpdateCamera(1);
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);
	engine.physics->debugDrawer.setDebugMode(btIDebugDraw::DBG_NoDebug);

	fallRigidBody = engine.physics->createBox(100,100,100,0,0,0,0);

	engine.getRenderer()->setWireframeMode(false);

	NMS_PointLight light1 = NMS_PointLight(GL_LIGHT0);
	light1.setAmbient(1,1,1,0.2);
	light1.setDiffuse(1,1,1,1);
	light1.setSpecular(1, 1, 1, 1);

	NMS_Sphere sphere = NMS_Sphere();

	MD2Model model = MD2Model();
	//""
	model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model.material.ambient = Vector(0.2, 0.2, 0.2, 0.2);
	model.material.diffuse = Vector(0.5, 0.5, 0.5, 0.5);
	model.material.specular = Vector(1, 1, 1, 1);
	model.material.shininess = 10;
	model.material.sphereMap = "textures/wikimapsphere.tga";

	sphere.material.ambient = Vector(0.5, 0.2, 0.2);
	sphere.material.diffuse = Vector(0.5, 0.5, 0.5);
	sphere.material.specular = Vector(1,1, 1, 1);
	sphere.material.shininess = 20;
	sphere.material.sphereMap = "textures/wikimapsphere.tga";

	GeometryNode GeoCube = GeometryNode(&sphere, fallRigidBody);
	GeometryNode freak = GeometryNode(&model, fallRigidBody);

	GeometryNode light = GeometryNode(&light1,fallRigidBody);
	SceneGraphNode* root = engine.getScene();

	Matrix tra = Matrix();
	Vector v = Vector(0.f, 250.f, 150.f);
	tra.translate(v);

	Matrix tra2 = Matrix();
	v = Vector(0.f, 120.f, 0.f);
	tra2.translate(v);

	traNode = TransformationNode(tra);
	traNode2 = TransformationNode(tra2);

	cam = NMSCameraFPS();

	root->addChild(&traNode);
	root->addChild(&traNode2);
	root->addChild(&GeoCube);
	traNode.addChild(&cam);
	traNode2.addChild(&freak);
	traNode.addChild(&light);

	NMS_EVENT_MANAGER.onKeyPressed(&keyPressed);
	NMS_EVENT_MANAGER.onKeyReleased(&keyReleased);
	NMS_EVENT_MANAGER.onMouseMoved(&mouseMoved);
	NMS_EVENT_MANAGER.onIdle(&idle);

	NMS_SceneRenderer* renderer = engine.getRenderer();
	renderer->setCurrentCamera(&cam);
	renderer->enableSkyBox("textures/world.tga");
	//renderer->setShaders("shaders\\fixedfunction.vertex", "shaders\\fixedfunction.fragment");
	renderer->setShaders("shaders\\spheremap.vert", "shaders\\spheremap.frag");

	engine.run();
	return 0;
}