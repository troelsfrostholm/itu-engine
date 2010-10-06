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
TransformationNode rotNode;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;
TransformationNode camTNode;
btRigidBody* fallRigidBody;
btRigidBody* fallRigidBody2;
btRigidBody* fallRigidBody3;
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

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	btQuaternion q;
	q.setEuler(0, 0.25, -0.05);

	btCollisionShape* fallShape = new btBoxShape(btVector3(100,100,100));
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(q,btVector3(0,0,0)));
    btScalar mass = 0.0f;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    engine.physics->addRBody(fallRigidBody);

	btCollisionShape* fallShape2 = new btBoxShape(btVector3(8,8,8));
	btDefaultMotionState* fallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,200,0)));
    mass = 1.0f;
    btVector3 fallInertia2(0,0,0);
    fallShape2->calculateLocalInertia(mass,fallInertia2);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass,fallMotionState2,fallShape2,fallInertia2);
    fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);
    engine.physics->addRBody(fallRigidBody2);

	MD2Model model = MD2Model();
	ColladaModel model2 = ColladaModel();
	model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model2.LoadModel("models/Duck/Duck.dae");
	model.SetAnim(RUN);

	//LIGHT DEFINITION
	LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,0,0,1));
	light0.defineLight(light0);
	AmbientLight light1 = AmbientLight();
	light1.setGlobalAmbient(&Vector(1,1,1,0));
	
	NMS_Cube cube = NMS_Cube();
	GeometryNode GeoCube = GeometryNode(&cube, fallRigidBody);

	NMS_Cube cube2 = NMS_Cube();
	GeometryNode GeoCube2 = GeometryNode(&cube2, fallRigidBody2);

	geom = GeometryNode(&model2, fallRigidBody2);
	GeometryNode light = GeometryNode(&light0,fallRigidBody);
	SceneGraphNode* root = engine.getScene();
	
	Matrix tra = Matrix();
	Vector v = Vector(0.f, 100.f, 300.f);
	tra.translate(v);

	Matrix tra2 = Matrix();
	v = Vector(0.f, 18.f, 0.f);
	tra2.translate(v);

	traNode = TransformationNode(tra);
	traNode2 = TransformationNode(tra2);


	cam = NMSCameraFPS();

	root->addChild(&traNode);
	root->addChild(&traNode2);
	traNode.addChild(&cam);
	root->addChild(&geom);
	traNode2.addChild(&GeoCube);
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