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
TransformationNode traNode2;
TransformationNode traNode3;
TransformationNode rotNode;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;
TransformationNode camTNode;
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

	NxActor *a1;
	NxActorDesc ad1;

	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions = NxVec3( 2.0f,  3.0f, 4.0f );

	ad1.shapes.pushBack(&boxDesc);
	ad1.density = 0.0f;
	ad1.globalPose.t = NxVec3( 10.0f , 10.0f, 10.0f );

	a1 = engine.physics->getScene()->createActor(ad1);
	a1->userData = NULL;

	NxActor *a2;
	NxActorDesc ad2;

	NxBoxShapeDesc boxDesc2;
	boxDesc2.dimensions = NxVec3( 2.0f,  3.0f, 4.0f );

	ad2.shapes.pushBack(&boxDesc2);
	ad2.density = 10.0f;
	ad2.globalPose.t = NxVec3( 10.0f , 10.0f, 10.0f );

	a2 = engine.physics->getScene()->createActor(ad2);
	a2->userData = NULL;

	MD2Model model = MD2Model();
	ColladaModel model2 = ColladaModel();
	//model.LoadModel("models/drfreak/drfreak.md2","models/drfreak/drfreak.tga");
	//model2.LoadModel("models/FireSpocket/models/FireSpocket.dae");
	model2.LoadModel("models/Duck/Duck.dae");
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
	GeometryNode GeoCube = GeometryNode(&cube);
	GeoCube.setActor(a1);

	NMS_Cube cube2 = NMS_Cube();
	GeometryNode GeoCube2 = GeometryNode(&cube2);

	geom = GeometryNode(&model2);
	geom.setActor(a2);
	GeometryNode light = GeometryNode(&light1);
	SceneGraphNode* root = engine.getScene();

	Matrix tra = Matrix();
	Vector v = Vector(0.f, 100.f, 300.f);
	tra.translate(v);

	Matrix tra2 = Matrix();
	v = Vector(0.f, 18.f, 0.f);
	tra2.translate(v);

	Matrix tra3 = Matrix();
	tra3.uScale(1.00);

	traNode = TransformationNode(tra);
	traNode2 = TransformationNode(tra2);
	traNode3 = TransformationNode(tra3);

	cam = NMSCameraFPS();

	root->addChild(&traNode);
	root->addChild(&traNode2);
	root->addChild(&traNode3);
	traNode.addChild(&cam);
	traNode3.addChild(&geom);
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