#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "MD2Model.h"
#include <cmath>
#include "NMS_Mutex.h"

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

	Mesh model = Mesh();
	GeometryNode geom = GeometryNode(&model);
	GeometryNode satelite = GeometryNode(&model);
	SceneGraphNode* root = engine.getScene();
	Matrix tra = Matrix();
	Vector v = Vector(0.f, 0.f, -10.f);
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