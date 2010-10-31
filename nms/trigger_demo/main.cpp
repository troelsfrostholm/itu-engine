#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "nms_physics.h"
#include "MD2Model.h"
#include <cmath>
#include "NMS_Mutex.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include "NMS_Audio.h"

#define WIDTH  640
#define HEIGHT  480
//NMS_Framework engine = NMS_Framework();

TransformationNode traNode;
TransformationNode rotNode;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;
btRigidBody* fallRigidBody;
btRigidBody* fallRigidBody2;
GeometryNode geom;
GeometryNode satelite;
NMS_Audio audioEngine;
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
	}
}

void idle( int i )
{
	cam.UpdateCamera(1);
}

void playSoundTrigger( int i )
{
	audioEngine.playSound("Test");
	fallRigidBody->setLinearVelocity(btVector3(0, 20, 0));
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	btCollisionShape* fallShape = new btBoxShape(btVector3(2,2,2));
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btScalar mass = 1.0f;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    engine.physics->addRBody(fallRigidBody);

	engine.physics->createTrigger(new btSphereShape(4), btTransform(btQuaternion(0,0,0,1),btVector3(0,-20,0)), &playSoundTrigger);

	MD2Model model = MD2Model();

	model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");

	geom = GeometryNode(&model, fallRigidBody);
	SceneGraphNode* root = engine.getScene();

	//LIGHT DEFINITION
	LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(20,0,0,1));
	light0.defineLight(light0);

	//SOUND DEFINITION
	ALfloat sourcePos[] = {0.0f,0.0f,0.0f};
	ALfloat sourceVel[] = {0.0f,0.0f,0.0f};
	audioEngine.LoadWav("sounds/test.wav","Test",sourcePos,sourceVel,1.0f,0.3f,false);

	GeometryNode light = GeometryNode(&light0,fallRigidBody);

	cam = NMSCameraFPS();
	Vector camTV = Vector(50.f, 2.f, 5000.f);
	Matrix camTM = Matrix();
	camTM.translate(camTV);
	cam.multiply(camTM);

	Matrix tra = Matrix();
	Vector v = Vector(-40.f, 0.f, 100.f);
	tra.translate(v);

	traNode = TransformationNode(tra);

	root->addChild(&geom);
	root->addChild(&light);
	root->addChild(&traNode);
	traNode.addChild(&cam);

	NMS_EVENT_MANAGER.onKeyPressed(&keyPressed);
	NMS_EVENT_MANAGER.onKeyReleased(&keyReleased);
	NMS_EVENT_MANAGER.onMouseMoved(&mouseMoved);
	NMS_EVENT_MANAGER.onIdle(&idle);

	NMS_SceneRenderer* renderer = engine.getRenderer();
	renderer->setCurrentCamera(&cam);
	
	engine.run();
	return 0;
}