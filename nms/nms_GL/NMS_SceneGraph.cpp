#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{

}

void SceneGraphNode::addChild(SceneGraphNode &child)
{
	children.push_back(&child);
}

void SceneGraphNode::traverse_df(SceneGraphVisitor v)
{
	Matrix m = Matrix();
	traverse_df(v, &m);
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

GeometryNode::GeometryNode(MD2Model *m)
{
	model = m;
}

void GeometryNode::before(SceneGraphVisitor v, Matrix *m)
{
	v.sg_before(*m, *model);
}

void GeometryNode::after(SceneGraphVisitor v, Matrix *m) 
{
	v.sg_after(*m, *model);
}

void SceneGraphVisitor::sg_before(Matrix transform, MD2Model model)
{
	anim_t list = model.animlist[0];

	cout << "Transformation : " << transform << endl;
	cout << "Framerate : " << list.fps << endl;
}

void SceneGraphVisitor::sg_after(Matrix transform, MD2Model model)
{
	cout << "callback after invoked with arg " << transform;
}