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
	void sg_before(Matrix t, NMS_Mesh* model, btRigidBody *b) 
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

	//btRigidBody rb = btRigidBody();
	btCollisionShape* fallShape2 = new btBoxShape(btVector3(2,2,2));
	btDefaultMotionState* fallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,-10)));
    float mass = 1.0f;
    btVector3 fallInertia2(0,0,0);
    fallShape2->calculateLocalInertia(mass,fallInertia2);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass,fallMotionState2,fallShape2,fallInertia2);
    btRigidBody* fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);

	//GeometryNode::GeometryNode(NMS_Mesh *m, btRigidBody *b);
	GeometryNode g1 = GeometryNode(model, fallRigidBody2);
	GeometryNode g2 = GeometryNode(model, fallRigidBody2);

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
	GeometryNode g1 = GeometryNode();
	GeometryNode g2 = GeometryNode();

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