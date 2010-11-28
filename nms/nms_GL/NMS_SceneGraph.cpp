#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{
	children = vector<SceneGraphNode*>();
	parent = NULL;
}

void SceneGraphNode::addChild(SceneGraphNode* child)
{
	children.push_back(child);
	child->setParent(this);
}

bool SceneGraphNode::isRoot()
{
	if(this->parent==NULL)
		return true;
	return false;
}

bool SceneGraphNode::isLeaf()
{
	return children.size() <= 0;
}

Vector TransformationNode::getWorldPosition(void)
{
	TransformationNode * nodePointer = this;
	Vector worldPosition;
	Vector temp;

	while(true)
	{
		
		temp = nodePointer->transform.getCol(4);

		if((!nodePointer->isRoot())&&(nodePointer->parent->isRoot()))
		{
			worldPosition = worldPosition + temp;
			break;
		}
		else if (!nodePointer->isRoot())
		{
			nodePointer = (TransformationNode*) nodePointer->parent;
			worldPosition = worldPosition + temp;
			worldPosition = worldPosition*(nodePointer->transform.getRotation());
		}
		else
			break;
	}

	return worldPosition;
}

void SceneGraphNode::setParent(SceneGraphNode* _parent)
{
	parent = _parent;
}

void SceneGraphNode::traverse_df(SceneGraphVisitor *v)
{
	Matrix m = Matrix();
	traverse_df(v, &m);
}

void SceneGraphNode::traverse_df(SceneGraphVisitor *v, Matrix *m)
{
	before(v, m);
	for(unsigned int i = 0; i < children.size(); i++) {
		children[i]->traverse_df(v, m);
	}
	after(v, m);
}

void SceneGraphNode::backtrack_to_root(SceneGraphVisitor *v, Matrix *m)
{
	if(!parent)  {  //This is the root node
		return;
	}
	after(v, m);
	parent->backtrack_to_root(v, m);
}

TransformationNode::TransformationNode() : SceneGraphNode()
{

}

TransformationNode::TransformationNode(Matrix t)
{
	transform = t;
}

Matrix TransformationNode::getTransformation()
{
	return transform;
}

void TransformationNode::multiply(Matrix m)
{
	transform = transform * m;
}

void TransformationNode::before(SceneGraphVisitor *v, Matrix *m)
{
	*m *= transform;
}

void TransformationNode::after(SceneGraphVisitor *v, Matrix *m)
{
	*m *= !transform;
}

GeometryNode::GeometryNode() : TransformationNode()
{ 
	model = NULL;
}

GeometryNode::GeometryNode(NMS_Mesh *m, btRigidBody *b) : TransformationNode()
{
	model = m;
	collisionBody = b;
}

GeometryNode::GeometryNode(NMS_Mesh *m, btRigidBody *b,  Matrix t) : TransformationNode(t) 
{ 
	model = m;
	collisionBody = b;
}

void GeometryNode::before(SceneGraphVisitor *v, Matrix *m)
{
	TransformationNode::before(v, m);
	v->sg_before(*m, this);
}

void GeometryNode::after(SceneGraphVisitor *v, Matrix *m) 
{
	v->sg_after(*m, this);
	TransformationNode::after(v, m);
}

NMS_Mesh * GeometryNode::getModel()
{
	return model;
}

btRigidBody * GeometryNode::getCollisionBody()
{
	return collisionBody;
}