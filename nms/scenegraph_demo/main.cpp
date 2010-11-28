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
TransformationNode rotNode;
TransformationNode rotNode2;
TransformationNode rotNode3;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;
TransformationNode sateliteTNode2;
TransformationNode camTNode;
btRigidBody* fallRigidBody;
btRigidBody* fallRigidBody2;
btRigidBody* fallRigidBody3;
GeometryNode geom;
GeometryNode satelite;
GeometryNode moon;
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
		case SDLK_UP:
			//Matrix m = Matrix();
			//m.rotY(0.1f);
			//SDL_LockMutex(sceneGraphGuard);
			//rotNode.multiply(m);
			//SDL_UnlockMutex(sceneGraphGuard);
			break;

		 case SDLK_a:
			 cam.setSlideSpeed(+0.5f);
				   break;
		 case SDLK_d:
			 cam.setSlideSpeed(-0.5f);
				   break;
		 case SDLK_w:
			 cam.setSpeed(+0.5f);
				   break;
		 case SDLK_s:
			 cam.setSpeed(-0.5f);
				   break;
	}
}

void idle( int i )
{
	cam.UpdateCamera(1);

	Matrix roty = Matrix();
	roty.rotY(0.1f);
	rotyNode.multiply(roty);
	Matrix rotx = Matrix();
	rotx.rotX(0.1f);
	rotNode3.multiply(rotx);
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);
	
	btCollisionShape* fallShape = new btBoxShape(btVector3(2,2,2));
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btScalar mass = 0.0f;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    engine.physics->addRBody(fallRigidBody);

	MD2Model model = MD2Model();
	MD2Model model2 = MD2Model();

	model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model2.LoadModel("models/hobgoblin/hobgoblin.md2","models/hobgoblin/hobgoblin.png");
	model.SetAnim(RUN);
	model2.SetAnim(JUMP);

	//LIGHT DEFINITION
	LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,-50,0,1));
	light0.defineLight(light0);

	geom = GeometryNode(&model, fallRigidBody);
	satelite = GeometryNode(&model2, fallRigidBody);
	moon = GeometryNode(&model, fallRigidBody);
	GeometryNode light = GeometryNode(&light0,fallRigidBody);
	SceneGraphNode* root = engine.getScene();
	
	Matrix tra = Matrix();
	Vector v = Vector(0.f, 0.f, -200.f);
	tra.translate(v);
	Matrix rot = Matrix();
	rot.rotX(45.f);
	Matrix roty = Matrix();
	roty.rotY(30.f);
	Matrix rot2 = Matrix();
	rot2.rotX(90.f);
	Matrix tra2 = Matrix();
	v = Vector(0.f, 0.f, -100.f);
	tra2.translate(v);

	traNode = TransformationNode(tra);
	rotNode = TransformationNode(rot);
	rotNode2 = TransformationNode(rot2);
	rotNode3 = TransformationNode(rot2);
	rotyNode = TransformationNode(roty);
	sateliteRNode = TransformationNode(roty);
	sateliteTNode = TransformationNode(tra2);
	sateliteTNode2 = TransformationNode(tra2);

	cam = NMSCameraFPS();
	Vector camTV = Vector(0.f, 2.f, 5.f);
	Matrix camTM = Matrix();
	camTM.translate(camTV);
	cam.multiply(camTM);

	root->addChild(&traNode);
	root->addChild(&cam);
	traNode.addChild(&light);
	traNode.addChild(&rotNode);
	rotNode.addChild(&rotyNode);
	rotyNode.addChild(&geom);
	geom.addChild(&sateliteTNode);
	sateliteTNode.addChild(&sateliteRNode);
	sateliteRNode.addChild(&satelite);
	satelite.addChild(&rotNode2);
	rotNode2.addChild(&sateliteTNode2);
	sateliteTNode2.addChild(&rotNode3);
	rotNode3.addChild(&moon);
	
	NMS_EVENT_MANAGER.onKeyPressed(&keyPressed);
	NMS_EVENT_MANAGER.onKeyReleased(&keyReleased);
	NMS_EVENT_MANAGER.onMouseMoved(&mouseMoved);
	NMS_EVENT_MANAGER.onIdle(&idle);

	NMS_SceneRenderer* renderer = engine.getRenderer();
	renderer->setCurrentCamera(&cam);
	renderer->setShaders("shaders\\fixedfunction.vertex", "shaders\\fixedfunction.fragment");
	
	engine.run();
	return 0;
}