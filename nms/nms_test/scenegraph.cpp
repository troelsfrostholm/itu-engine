#include "NMS_SceneGraph.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace std;
//using namespace nms;

BOOST_AUTO_TEST_SUITE( scenegraph );

BOOST_AUTO_TEST_CASE( scenegraph_node )
{
	//BOOST_CHECK( approximates(1.01f, 1.00f, 0.02f) );
	/*Matrix t = Matrix();
	t.uScale(2.0f);
	cout << t << endl;
	cout << !t << endl;
	TransformationNode s = TransformationNode(t);
	TransformationNode s2 = TransformationNode(t);
	TransformationNode s3 = TransformationNode(t);
	TransformationNode s4 = TransformationNode(t);
	TransformationNode s5 = TransformationNode(t);

	s.addChild(s2);
	s2.addChild(s3);
	s.addChild(s4);
	s4.addChild(s5);

	Visitor v = Visitor();
	Matrix m = Matrix();
	s.traverse_df(v, &m);*/

}

BOOST_AUTO_TEST_SUITE_END();