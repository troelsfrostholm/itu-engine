#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{

}

void SceneGraphNode::addChild(SceneGraphNode &child)
{
	children.push_back(&child);
}

void SceneGraphNode::traverse_df(SceneGraphVisitor v, Matrix *m)
{
	before(v, m);
	for(unsigned int i = 0; i < children.size(); i++) {
		children[i]->traverse_df(v, m);
	}
	after(v, m);
}

TransformationNode::TransformationNode(Matrix t)
{
	transform = t;
}

void TransformationNode::before(SceneGraphVisitor v, Matrix *m)
{
	*m *= transform;
}

void TransformationNode::after(SceneGraphVisitor v, Matrix *m)
{
	*m *= (!transform);
}

void GeometryNode::before(SceneGraphVisitor v, Matrix *m)
{
	v.sg_after(*m);
}

void GeometryNode::after(SceneGraphVisitor v, Matrix *m) 
{
	v.sg_after(*m);
}

void SceneGraphVisitor::sg_before(Matrix arg)
{
	cout << "callback before invoked with arg " << arg;
}

void SceneGraphVisitor::sg_after(Matrix arg)
{
	cout << "callback after invoked with arg " << arg;
}