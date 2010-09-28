#include "NMS_Framework.h"
#include "MD2Model.h"
#include <cmath>

#define WIDTH  640
#define HEIGHT  480
NMS_Framework engine = NMS_Framework();

int main(int argc, char* argv[])
{
	engine.NMSInit(WIDTH, HEIGHT, 16, "Demo 2", false);
	//MD2Model obj = MD2Model();
	/*obj.LoadModel("models/Baron/BaronBody.md2");
	obj.LoadSkin("models/Baron/baron.jpg");
	obj.SetAnim(BOOM);*/
	//GeometryNode* baron = new GeometryNode(&obj);
	Mesh model = Mesh();
	GeometryNode* geom = new GeometryNode(&model);
	SceneGraphNode* root = engine.getScene();
	Matrix m = Matrix();
	root->addChild(new TransformationNode(m));
	//root->addChild(geom);
	/*root = NULL;
	baron = NULL;*/
	engine.run();
	return 0;
}