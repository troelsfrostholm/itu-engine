#include "NMS_Engine.h"
#include "NMS_Framework.h"
#include "MD2Model.h"
#include <cmath>

#define WIDTH  640
#define HEIGHT  480
//NMS_Framework engine = NMS_Framework();

int main(int argc, char* argv[])
{
	/*
	InitEngine(WIDTH, HEIGHT, 16, "Demo 2", false);
	Mesh model = Mesh();
	SceneGraphNode* root = engine.getScene();
	Matrix m = Matrix();
	m.translate(Vector(2.f, 0, 0));
	TransformationNode* transform = new TransformationNode(m);
	GeometryNode* geom = new GeometryNode(&model);
	root->addChild(geom);
	StartEngine();
	*/
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);
	/*
	MD2Model obj = MD2Model();
	obj.LoadModel("models/Baron/BaronBody.md2");
	obj.LoadSkin("models/Baron/baron.jpg");
	obj.SetAnim(BOOM);
	GeometryNode* baron = new GeometryNode(&obj);
	*/
	Mesh model = Mesh();
	GeometryNode geom = GeometryNode(&model);
	SceneGraphNode* root = engine.getScene();
	Matrix m = Matrix();
	Vector v = Vector(1.f, 1.f, 0.f);
	//m.rotZ(45.f);
	m.rotX(5.f);
	m.debugPrint();
	TransformationNode t = TransformationNode(m);
	root->addChild(&t);
	t.addChild(&geom);
	
	/*root = NULL;
	baron = NULL;*/
	engine.run();
	return 0;
}