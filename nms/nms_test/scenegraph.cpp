#include <boost/test/unit_test.hpp>
#include <iostream>
#include "NMS_Mesh.h"
#include "NMS_SceneGraph.h"


using namespace std;
//using namespace nms;

BOOST_AUTO_TEST_SUITE( scenegraph );

class SV : public SceneGraphVisitor
{
public:
	void sg_before(Matrix t, NMS_Mesh* model) 
	{
		cout << "Called back with matrix: " << t << endl;
		(*model).render(0);
	}
	void sg_after(Matrix t, NMS_Mesh* model) {
	cout << "Called back with matrix: " << t << endl;
		(*model).render(0);
	}
};

BOOST_AUTO_TEST_CASE( scenegraph_node )
{

	NMS_Mesh* model = &MD2Model();

	Matrix t = Matrix();
	Matrix root_t = Matrix();
	Vector vec = Vector(1.f, 0.f, 0.f);
	t.translate(vec);
	cout << "t is " << endl << t << endl;
	cout << "inverse t is " << endl << !t << endl;
	TransformationNode s = TransformationNode(root_t);
	TransformationNode s2 = TransformationNode(t);
	TransformationNode s3 = TransformationNode(t);
	TransformationNode s4 = TransformationNode(t);
	TransformationNode s5 = TransformationNode(t);
	GeometryNode g1 = GeometryNode(&model, NULL);
	GeometryNode g2 = GeometryNode(&model, NULL);

	s.addChild(&s2);
	s2.addChild(&s3);
	s3.addChild(&s4);
	s.addChild(&s5);
	s4.addChild(&g1);
	s5.addChild(&g2);

	SV v = SV();
	s.traverse_df(&v);
}

BOOST_AUTO_TEST_CASE( scenegraph_backtrack_to_root )
{
	cout << endl << endl << "Examining scene graph backtracking" << endl;

	Mesh model = Mesh();

	Matrix t = Matrix();
	Matrix root_t = Matrix();
	Vector vec = Vector(1.f, 0.f, 0.f);
	t.translate(vec);
	cout << "t is " << endl << t << endl;
	cout << "inverse t is " << endl << !t << endl;
	TransformationNode s = TransformationNode(root_t);
	TransformationNode s2 = TransformationNode(t);
	TransformationNode s3 = TransformationNode(t);
	TransformationNode s4 = TransformationNode(t);
	TransformationNode s5 = TransformationNode(t);
	GeometryNode g1 = GeometryNode(&model);
	GeometryNode g2 = GeometryNode(&model);

	s.addChild(&s2);
	s2.addChild(&s3);
	s3.addChild(&s4);
	s.addChild(&s5);
	s4.addChild(&g1);
	s5.addChild(&g2);

	SV v = SV();
	Matrix temp = Matrix();
	cout << "Base case: Backtrack from root to root (do nothing)" << endl;
	s.backtrack_to_root(&v, &temp);
	cout << "Result ends up as : " << endl << temp << endl << endl;
	cout << "Backtrack from deepest node in tree" << endl;
	Matrix temp2 = Matrix();
	g1.backtrack_to_root(&v, &temp2);
	cout << "Result ends up as : " << endl << temp2 << endl << endl;
}

BOOST_AUTO_TEST_SUITE_END();