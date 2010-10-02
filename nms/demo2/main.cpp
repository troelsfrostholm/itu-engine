#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "MD2Model.h"
#include <cmath>

#define WIDTH  640
#define HEIGHT  480
//NMS_Framework engine = NMS_Framework();

TransformationNode traNode;
TransformationNode rotNode;
TransformationNode rotyNode;


void keyPressed(SDLKey key)
{
	switch( key ) {
		case SDLK_UP:
			Matrix m = Matrix();
			m.rotY(5.f);
			rotNode.multiply(m);
			break;
	}
}

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);

	Mesh model = Mesh();
	GeometryNode geom = GeometryNode(&model);
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

	root->addChild(&traNode);
	traNode.addChild(&rotNode);
	rotNode.addChild(&rotyNode);
	rotyNode.addChild(&geom);

	NMS_EVENT.onKeyPressed(&keyPressed);
	
	engine.run();
	return 0;
}