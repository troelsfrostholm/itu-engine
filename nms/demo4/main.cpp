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
CameraNode cam;
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

void keyReleased(SDLKey key)
{
	SDL_LockMutex(inputGuard);
	switch( key ) {
		case SDLK_a:
			 fpsCam.setSlideSpeed(0);
			 break;
		case SDLK_d:
			 fpsCam.setSlideSpeed(0);
			 break;
		case SDLK_w:
			 fpsCam.setSpeed(0);
			 break;
		case SDLK_s:
			 fpsCam.setSpeed(0);
			 break;
		case SDLK_UP:
			//((NMS_KinematicMotionState*) fallRigidBody->getMotionState())->setKinematicPos( btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)) );
			break;
	}
	SDL_UnlockMutex(inputGuard);
}


void keyPressed(SDLKey key)
{
	SDL_LockMutex(inputGuard);
	switch( key ) {
		case SDLK_UP:
			//Matrix m = Matrix();
			//m.rotY(0.1f);
			//SDL_LockMutex(sceneGraphGuard);
			//rotNode.multiply(m);
			//SDL_UnlockMutex(sceneGraphGuard);
			break;

		 case SDLK_a:
			 fpsCam.setSlideSpeed(+0.01f);
				   break;
		 case SDLK_d:
			 fpsCam.setSlideSpeed(-0.01f);
				   break;
		 case SDLK_w:
			 fpsCam.setSpeed(+0.01f);
				   break;
		 case SDLK_s:
			 fpsCam.setSpeed(-0.01f);
				   break;
		 case SDLK_1:
			 renderer->setCurrentCamera(&cam);
			 break;
		 case SDLK_2:
			 renderer->setCurrentCamera(&fpsCam);
			 break;

	}
	SDL_UnlockMutex(inputGuard);
}

void idle( int i )
{
	fpsCam.UpdateCamera(1);
	//((NMS_KinematicMotionState*) fallRigidBody->getMotionState())->setKinematicPos( btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)) );
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	TransformationNode traNode3;
	ColladaModel model3 = ColladaModel();
	//model3.LoadModel("models/Gundam/Gundam.dae");
	//model3.LoadModel("models/FireSpocket/models/FireSpocket.dae");
	model3.LoadModel("models/Astroboy/astroBoy_walk_Max.DAE");
	//model3.LoadModel("models/Duck/Duck.dae");
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
	
	cam = CameraNode();
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
	renderer->setWireframeMode(true);


	NMS_EVENT_MANAGER.onKeyPressed(&keyPressed);
	NMS_EVENT_MANAGER.onKeyReleased(&keyReleased);
	NMS_EVENT_MANAGER.onMouseMoved(&mouseMoved);
	NMS_EVENT_MANAGER.onIdle(&idle);
	engine.run();
	return 0;
}