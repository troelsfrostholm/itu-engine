#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "nms_physics.h"
#include "MD2Model.h"
#include "ColladaModel.h"
#include <cmath>
#include "NMS_Mutex.h"
#include "NMS_LightSystem.h"
#include "NMS_Audio.h"
#include "NMS_Camera.h"
#include "Quaternion.h"

#define WIDTH 600
#define HEIGHT 400

NMSCameraFPS fpsCam;
NMSCameraFPS cam;
NMS_SceneRenderer* renderer;

void mouseMoved(int MouseX, int MouseY)
{
	Vector rot=fpsCam.getRotation();
	//Pitch rotation, with the mouse is up and down
	rot[NMS_X]+=(MouseY)*0.03f;
	//Yaw rotation, with the mouse you can do it using the left right position
	rot[NMS_Y]+=(MouseX)*0.03f;	
	fpsCam.setRotation(rot[NMS_X],rot[NMS_Y],rot[NMS_Z]);
}

void idle( int i )
{
	fpsCam.UpdateCamera(1);
	//((NMS_KinematicMotionState*) fallRigidBody->getMotionState())->setKinematicPos( btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)) );
}

void moveLeft( int i )
{
	fpsCam.setPos(fpsCam.getPos() + fpsCam.getRight()*0.5);
}

void moveRight( int i )
{
	fpsCam.setPos(fpsCam.getPos() - fpsCam.getRight()*0.5);
}

void moveForward( int i )
{
	fpsCam.setPos(fpsCam.getPos() + fpsCam.getDir()*0.5);
}

void moveBack( int i )
{
	fpsCam.setPos(fpsCam.getPos() - fpsCam.getDir()*0.5);
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	TransformationNode traNode3;
	ColladaModel model3 = ColladaModel();
	//model3.LoadModel("models/Gundam/Gundam.dae");
	//model3.LoadModel("models/FireSpocket/models/FireSpocket.dae");
	//model3.LoadModel("models/Goblin/Goblin.DAE");
	model3.LoadModel("models/Astroboy/Astroboy.DAE");
	//model3.LoadModel("models/Duck/Duck.dae");
	//model3.LoadModel("models/pumpkin/pumpkin.dae");
	float mass;
	btVector3 fallInertia;

	//Setup collition shapes
	btCollisionShape* staticShape = new btBoxShape(btVector3(20,2,20));
	btQuaternion incline = btQuaternion();
	incline.setEuler(0, 0.25, -0.05);
	btDefaultMotionState* staticMotionState = new btDefaultMotionState(btTransform(incline,btVector3(0,-20,-50)));
    mass = 0.0f;
    fallInertia = btVector3(0,0,0);
    staticShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo staticRigidBodyCI2(mass,staticMotionState,staticShape,fallInertia);
    btRigidBody* staticRigidBody = new btRigidBody(staticRigidBodyCI2);
    engine.physics->addRBody(staticRigidBody);

	btCollisionShape* fallShape = new btBoxShape(btVector3(2,2,2));
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,-50)));
    mass = 0.0f;
    fallInertia = btVector3(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI2);
    engine.physics->addRBody(fallRigidBody);

	//LIGHT DEFINITION
	LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,0,0,1));
	light0.defineLight(light0);
	
	//Create scene graph
	NMS_VerticalPlane plane = NMS_VerticalPlane();

	SceneGraphNode* root = engine.getScene();
	GeometryNode freakNode = GeometryNode(&model3, fallRigidBody);
	GeometryNode planeNode = GeometryNode(&plane, staticRigidBody);
	GeometryNode light	   = GeometryNode(&light0,staticRigidBody);

	Vector vec = Vector(0.f, 0.f, -50.f);
	Matrix transl = Matrix();
	transl.translate(vec);
	TransformationNode tNode = TransformationNode(transl);
	transl.translate(Vector(0.f, -20.f, -50.f));
	TransformationNode planeTNode = TransformationNode(transl);
	
	cam = NMSCameraFPS();
	transl.translate(Vector(0.f, -10.f, 0.f));
	Matrix rot = Matrix();
	rot.rotY(120.f);
	//cam.multiply(rot);
	cam.multiply(transl);
	
	Matrix tra3 = Matrix();
	tra3.uScale(1.0f);

	traNode3=TransformationNode(tra3);

	root->addChild(&light);
	root->addChild(&tNode);
	tNode.addChild(&traNode3);
	root->addChild(&planeTNode);
	traNode3.addChild(&freakNode);
	planeTNode.addChild(&planeNode);
	root->addChild(&cam);

	//Setup camera
	fpsCam = NMSCameraFPS();
	root->addChild(&fpsCam);
	renderer = engine.getRenderer();
	renderer->setCurrentCamera(&fpsCam);
	renderer->setWireframeMode(false);
	renderer->setShaders("shaders\\fixedfunction.vertex", "shaders\\fixedfunction.fragment");

	NMS_EVENT_MANAGER.onMouseMoved(&mouseMoved);
	NMS_EVENT_MANAGER.onIdle(&idle);
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_a, "left");
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_d, "right");
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_w, "up");
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_s, "down");

	NMS_EVENT_MANAGER.bindKeyHold(SDLK_LEFT, "left");
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_RIGHT, "right");
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_UP, "up");
	NMS_EVENT_MANAGER.bindKeyHold(SDLK_DOWN, "down");

	NMS_EVENT_MANAGER.bindKeyPress(SDLK_SPACE, "jump");

	NMS_EVENT_MANAGER.bindAction("left", &moveLeft);
	NMS_EVENT_MANAGER.bindAction("right", &moveRight);
	NMS_EVENT_MANAGER.bindAction("up", &moveForward);
	NMS_EVENT_MANAGER.bindAction("down", &moveBack);
	//NMS_EVENT_MANAGER.bindAction("jump", &toggleDebugDraw);

	engine.run();
	return 0;
}