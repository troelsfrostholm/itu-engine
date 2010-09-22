#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{

}

void SceneGraphNode::addChild(SceneGraphNode &child)
{
	children.push_back(&child);
}

void SceneGraphNode::traverse_df(Visitor v, Matrix *m)
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

void TransformationNode::before(Visitor v, Matrix *m)
{
	*m *= transform;
}

void TransformationNode::after(Visitor v, Matrix *m)
{
	*m *= (!transform);
	v.callback(*m);
}

void Visitor::callback(Matrix arg)
{
	cout << "callback invoked with arg " << arg;
}