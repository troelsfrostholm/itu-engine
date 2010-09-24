#include "NMS_SceneGraph.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace std;
//using namespace nms;

BOOST_AUTO_TEST_SUITE( scenegraph );

BOOST_AUTO_TEST_CASE( scenegraph_node )
{
	Matrix t = Matrix();
	t.uScale(2.0f);
	cout << t << endl;
	cout << !t << endl;
	TransformationNode s = TransformationNode(t);
	TransformationNode s2 = TransformationNode(t);
	TransformationNode s3 = TransformationNode(t);
	TransformationNode s4 = TransformationNode(t);
	TransformationNode s5 = TransformationNode(t);
	GeometryNode g1 = GeometryNode();
	GeometryNode g2 = GeometryNode();

	s.addChild(s2);
	s2.addChild(s3);
	s3.addChild(s4);
	s.addChild(s5);
	s4.addChild(g1);
	s5.addChild(g2);

	SceneGraphVisitor v = SceneGraphVisitor();
	Matrix m = Matrix();
	s.traverse_df(v, &m);

}

BOOST_AUTO_TEST_SUITE_END();