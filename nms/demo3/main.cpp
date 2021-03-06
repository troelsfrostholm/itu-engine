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

#define WIDTH 600
#define HEIGHT 400

NMSCameraFPS fpsCam;
NMSCameraController * cam;
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
			 fpsCam.setSlideSpeed(+0.5f);
				   break;
		 case SDLK_d:
			 fpsCam.setSlideSpeed(-0.5f);
				   break;
		 case SDLK_w:
			 fpsCam.setSpeed(+0.5f);
				   break;
		 case SDLK_s:
			 fpsCam.setSpeed(-0.5f);
				   break;
		 case SDLK_1:
			 renderer->setCurrentCamera(cam);
			 break;
		 case SDLK_2:
			 renderer->setCurrentCamera(&fpsCam);
			 break;

	}
}



void idle( int i )
{
	fpsCam.UpdateCamera(1);
	//((NMS_KinematicMotionState*) fallRigidBody->getMotionState())->setKinematicPos( btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)) );
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	//Load models
	MD2Model freakModel = MD2Model();
	MD2Model model2 = MD2Model();

	ColladaModel model3 = ColladaModel();
	freakModel.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model2.LoadModel("models/hobgoblin/hobgoblin.md2","models/hobgoblin/hobgoblin.png");
	model3.LoadModel("models/colladaDuck/duck_triangulate.dae");
	freakModel.SetAnim(RUN);
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
    mass = 1.0f;
    fallInertia = btVector3(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI2);
    engine.physics->addRBody(fallRigidBody);

	//LIGHT DEFINITION
	/*LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,0,0,1));
	light0.defineLight(light0);*/

	NMS_PointLight light0 = NMS_PointLight(GL_LIGHT0);
	light0.setAmbient(1,1,1,0.2);
	light0.setDiffuse(1,1,1,1);
	light0.setSpecular(1, 1, 1, 1);
	
	//Create scene graph
	NMS_Sphere plane = NMS_Sphere();
	plane.material.ambient = Vector(0, 0, 1, 1);
	plane.material.diffuse = Vector(0, 0, 1, 1);
	plane.material.specular = Vector(0, 0, 1, 1);

	SceneGraphNode* root = engine.getScene();
	GeometryNode freakNode = GeometryNode(&freakModel, fallRigidBody);
	GeometryNode planeNode = GeometryNode(&plane, staticRigidBody);
	GeometryNode light = GeometryNode(&light0,fallRigidBody);
	Vector vec = Vector(0.f, 0.f, -50.f);
	Matrix transl = Matrix();
	transl.translate(vec);
	TransformationNode tNode = TransformationNode(transl);
	transl.translate(Vector(0.f, -20.f, -50.f));
	TransformationNode planeTNode = TransformationNode(transl);
	cam = new NMSCameraFPS();
	transl.translate(Vector(0.f, -10.f, 0.f));
	Matrix rot = Matrix();
	rot.rotY(120.f);
	//cam.multiply(rot);
	cam->multiply(transl);
	

	root->addChild(&light);
	root->addChild(&tNode);
	root->addChild(&planeTNode);
	tNode.addChild(&freakNode);
	planeTNode.addChild(&planeNode);
	root->addChild(cam);

	//Setup camera
	fpsCam = NMSCameraFPS();
	root->addChild(&fpsCam);
	renderer = engine.getRenderer();
	renderer->setCurrentCamera(&fpsCam);
	renderer->setWireframeMode(false);
	renderer->setShaders("shaders\\fixedfunction.vertex", "shaders\\fixedfunction.fragment");

	NMS_EVENT_MANAGER.onKeyPressed(&keyPressed);
	NMS_EVENT_MANAGER.onKeyReleased(&keyReleased);
	NMS_EVENT_MANAGER.onMouseMoved(&mouseMoved);
	NMS_EVENT_MANAGER.onIdle(&idle);
	engine.run();
	return 0;
}