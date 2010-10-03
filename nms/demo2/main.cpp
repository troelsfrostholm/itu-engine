#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "MD2Model.h"
#include "ColladaModel.h"
#include <cmath>
#include "NMS_Mutex.h"
#include "NMS_LightSystem.h"

#define WIDTH  640
#define HEIGHT  480
//NMS_Framework engine = NMS_Framework();

TransformationNode traNode;
TransformationNode rotNode;
TransformationNode rotyNode;
TransformationNode sateliteRNode;
TransformationNode sateliteTNode;


void keyPressed(SDLKey key)
{
	switch( key ) {
		case SDLK_UP:
			Matrix m = Matrix();
			m.rotY(0.1f);
			SDL_LockMutex(sceneGraphGuard);
			rotNode.multiply(m);
			SDL_UnlockMutex(sceneGraphGuard);
			break;
	}
}

void idle( int i )
{
	Matrix m = Matrix();
	m.rotY(0.05f);
	SDL_LockMutex(sceneGraphGuard);
	rotNode.multiply(m);
	sateliteRNode.multiply(m);
	SDL_UnlockMutex(sceneGraphGuard);
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

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

	GeometryNode geom = GeometryNode(&model);
	GeometryNode satelite = GeometryNode(&model2);
	GeometryNode light = GeometryNode(&light0);
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

	root->addChild(&traNode);
	root->addChild(&light);
	traNode.addChild(&rotNode);
	rotNode.addChild(&rotyNode);
	rotyNode.addChild(&geom);
	geom.addChild(&sateliteTNode);
	sateliteTNode.addChild(&sateliteRNode);
	sateliteRNode.addChild(&satelite);
	
	NMS_EVENT.onKeyPressed(&keyPressed);
	NMS_EVENT.onIdle(&idle);
	
	engine.run();
	return 0;
}