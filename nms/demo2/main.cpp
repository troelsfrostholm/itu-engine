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
	Matrix tra = Matrix();
	Vector v = Vector(0.f, 0.f, -10.f);
	tra.translate(v);
	Matrix rot = Matrix();
	//m.rotZ(45.f);
	rot.rotX(45.f);
	Matrix roty = Matrix();
	roty.rotY(30.f);
	//rot.debugPrint();
	traNode = TransformationNode(tra);
	rotNode = TransformationNode(rot);
	rotyNode = TransformationNode(roty);

	root->addChild(&traNode);
	traNode.addChild(&rotNode);
	rotNode.addChild(&rotyNode);
	rotyNode.addChild(&geom);
	
	/*root = NULL;
	baron = NULL;*/
	engine.run();
	return 0;
}