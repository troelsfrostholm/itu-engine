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

void idle(int i)
{
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	//Load models
	MD2Model model = MD2Model();
	MD2Model model2 = MD2Model();

	ColladaModel model3 = ColladaModel();
	model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	model2.LoadModel("models/hobgoblin/hobgoblin.md2","models/hobgoblin/hobgoblin.png");
	model3.LoadModel("models/colladaDuck/duck_triangulate.dae");
	model.SetAnim(RUN);
	model2.SetAnim(JUMP);

	//Setup collition shapes
	btCollisionShape* fallShape2 = new btBoxShape(btVector3(2,2,2));
	btDefaultMotionState* fallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,-10)));
    float mass = 1.0f;
    btVector3 fallInertia2(0,0,0);
    fallShape2->calculateLocalInertia(mass,fallInertia2);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass,fallMotionState2,fallShape2,fallInertia2);
    btRigidBody* fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);
    engine.physics->addRBody(fallRigidBody2);
	
	//Create scene graph
	SceneGraphNode* root = engine.getScene();
	GeometryNode geom = GeometryNode(&model, fallRigidBody2);
	Vector vec = Vector(0.f, -10.f, -10.f);
	Matrix transl = Matrix();
	transl.translate(vec);
	geom.multiply(transl);
	root->addChild(&geom);
	
	//Setup camera
	NMSCameraFPS cam = NMSCameraFPS();
	Vector camTV = Vector(0.f, 0.f, 20.f);
	Matrix camTM = Matrix();
	camTM.translate(camTV);
	root->addChild(&cam);
	NMS_SceneRenderer* renderer = engine.getRenderer();
	//renderer->setCurrentCamera(&cam);

	//NMS_EVENT.onIdle(&idle);
	engine.run();
	return 0;
}