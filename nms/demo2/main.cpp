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

#define WIDTH  640
#define HEIGHT  480
//NMS_Framework engine = NMS_Framework();

TransformationNode traNode;
TransformationNode rotNode;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;
TransformationNode camTNode;
btRigidBody* fallRigidBody;
btRigidBody* fallRigidBody2;
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
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(q,btVector3(0,-10,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	engine.physics->addRBody(groundRigidBody);

	btCollisionShape* fallShape = new btSphereShape(4);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btScalar mass = 1.0f;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    engine.physics->addRBody(fallRigidBody);

	btCollisionShape* fallShape2 = new btBoxShape(btVector3(2,2,2));
	btDefaultMotionState* fallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,-10)));
    mass = 1.0f;
    btVector3 fallInertia2(0,0,0);
    fallShape2->calculateLocalInertia(mass,fallInertia2);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass,fallMotionState2,fallShape2,fallInertia2);
    fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);
    engine.physics->addRBody(fallRigidBody2);

	MD2Model model = MD2Model();
	MD2Model model2 = MD2Model();

	//ColladaModel model2 = ColladaModel();
	model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model2.LoadModel("models/hobgoblin/hobgoblin.md2","models/hobgoblin/hobgoblin.png");
	//model2.LoadModel("models/colladaDuck/duck_triangulate.dae");
	model.SetAnim(RUN);
	model2.SetAnim(JUMP);



	//LIGHT DEFINITION
	LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,-50,0,1));
	light0.defineLight(light0);

	//SOUND DEFINITION
	NMS_Audio audioEngine;
	ALfloat sourcePos[] = {0.0f,0.0f,0.0f};
	ALfloat sourceVel[] = {0.0f,0.0f,0.0f};
	audioEngine.LoadWav("sounds/BackgroundMusic.wav","Background",sourcePos,sourceVel,1.0f,0.3f,true);
	audioEngine.playSound("Background");
	

	geom = GeometryNode(&model, fallRigidBody);
	satelite = GeometryNode(&model2, fallRigidBody2);
	GeometryNode light = GeometryNode(&light0,fallRigidBody2);
	SceneGraphNode* root = engine.getScene();
	Matrix tra = Matrix();
	Vector v = Vector(0.f, 0.f, -50.f);
	tra.translate(v);
	Matrix rot = Matrix();
	rot.rotX(45.f);
	Matrix roty = Matrix();
	roty.rotY(30.f);

	traNode = TransformationNode(tra);
	rotNode = TransformationNode(rot);
	rotyNode = TransformationNode(roty);
	sateliteRNode = TransformationNode(roty);
	sateliteTNode = TransformationNode(tra);

	cam = NMSCameraFPS();
	Vector camTV = Vector(0.f, 2.f, 5.f);
	Matrix camTM = Matrix();
	camTM.translate(camTV);
	cam.multiply(camTM);

	root->addChild(&traNode);
	root->addChild(&light);
	traNode.addChild(&rotNode);
	rotNode.addChild(&rotyNode);
	rotyNode.addChild(&geom);
	geom.addChild(&sateliteTNode);
	geom.addChild(&cam);
	sateliteTNode.addChild(&sateliteRNode);
	sateliteRNode.addChild(&satelite);
	
	NMS_EVENT.onKeyPressed(&keyPressed);
	NMS_EVENT.onKeyReleased(&keyReleased);
	NMS_EVENT.onMouseMoved(&mouseMoved);
	NMS_EVENT.onIdle(&idle);

	NMS_SceneRenderer* renderer = engine.getRenderer();
	renderer->setCurrentCamera(&cam);
	
	engine.run();
	return 0;
}