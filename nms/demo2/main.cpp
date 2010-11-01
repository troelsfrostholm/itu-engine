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
	switch( key ) {
		 case SDLK_a:
			 cam.setSlideSpeed(+0.01f);
				   break;
		 case SDLK_d:
			 cam.setSlideSpeed(-0.01f);
				   break;
		 case SDLK_w:
			 cam.setSpeed(+0.01f);
				   break;
		 case SDLK_s:
			 cam.setSpeed(-0.01f);
				   break;
		 case SDLK_UP:
			 fallRigidBody2->activate();
			 fallRigidBody2->setWorldTransform(btTransform(btQuaternion(0,0,0,1),btVector3(0,200,40)));
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

	fallRigidBody = engine.physics->createBox(100,100,100,0,0,0,0);

	fallRigidBody2 = engine.physics->createBox(8,12,8,0,200,0, 1.0f);

	fallRigidBody3 = engine.physics->createBox(8,12,8,0,200,20, 1.0f);

	fallRigidBody4 = engine.physics->createBox(8,12,8,0,200,60, 1.0f);

	MD2Model model = MD2Model();
	ColladaModel model2 = ColladaModel();
	MD2Model model3 = MD2Model();
	//model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model.LoadModel("models/hang2/HANG2.md2","models/hang2/hang2.bmp");
	model3.LoadModel("models/hang3/HANG3.md2","models/hang3/hang3.bmp");
	//model2.LoadModel("models/pumpkin/pumpkin.dae");
	model.SetAnim(RUN);

	//LIGHT DEFINITION
	/*LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,0,0,1));
	light0.defineLight(light0);*/
	AmbientLight light1 = AmbientLight();
	light1.setGlobalAmbient(&Vector(1,1,1,0));

	NMS_Cube cube = NMS_Cube();
	GeometryNode GeoCube = GeometryNode(&cube, fallRigidBody);

	NMS_Cube cube2 = NMS_Cube();
	GeometryNode GeoCube2 = GeometryNode(&cube2, fallRigidBody2);

	GeometryNode geom2 = GeometryNode(&model3, fallRigidBody3);

	GeometryNode geom3 = GeometryNode(&model2, fallRigidBody4);

	geom = GeometryNode(&model, fallRigidBody2);
	GeometryNode light = GeometryNode(&light1,fallRigidBody);
	SceneGraphNode* root = engine.getScene();

	Matrix tra = Matrix();
	Vector v = Vector(0.f, 150.f, 150.f);
	tra.translate(v);

	Matrix tra2 = Matrix();
	v = Vector(0.f, 18.f, 0.f);
	tra2.translate(v);

	Matrix tra3 = Matrix();
	tra3.uScale(1.0);

	traNode = TransformationNode(tra);
	traNode2 = TransformationNode(tra2);
	traNode3 = TransformationNode(tra3);


	cam = NMSCameraFPS();

	root->addChild(&traNode);
	root->addChild(&traNode2);
	root->addChild(&traNode3);
	traNode.addChild(&cam);
	traNode3.addChild(&geom);
	traNode3.addChild(&geom2);
	traNode3.addChild(&geom3);
	//traNode2.addChild(&GeoCube);
	root->addChild(&light);

	NMS_EVENT.onKeyPressed(&keyPressed);
	NMS_EVENT.onKeyReleased(&keyReleased);
	NMS_EVENT.onMouseMoved(&mouseMoved);
	NMS_EVENT.onIdle(&idle);

	NMS_SceneRenderer* renderer = engine.getRenderer();
	renderer->setCurrentCamera(&cam);

	engine.run();
	return 0;
}