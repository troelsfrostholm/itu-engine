#include "NMS_Skeleton.h"


JointNode::JointNode() : TransformationNode() 
{
}

JointNode::JointNode(string sID, string sName, string sSID, string sType,Matrix t) : TransformationNode(t) 
{
	this->sID=sID;
	this->sName=sName;
	this->sSID=sSID;
	this->sType=sType;
}

JointNode* Skeleton::getJoint(string sID)
{
	return &joints[sID];
}


string JointNode::getSSID()
{
	return sSID;
}

Matrix JointNode::getTransform()
{
	return transform;
}


JointNode* JointNode::getParent()
{
	return (JointNode*)parent;
}

void JointNode::before(SceneGraphVisitor *v, Matrix *m)
{
	TransformationNode::before(v, m);
	v->sg_before(*m, this);
}

void JointNode::after(SceneGraphVisitor *v, Matrix *m) 
{
	v->sg_after(*m, this);
	TransformationNode::after(v, m);
}


SkeletonRenderer::SkeletonRenderer()
{
	startingPoint=Vector();
}

void SkeletonRenderer::sg_before(Matrix transform, SceneGraphNode * node)
{
	renderJoint(transform, node);
}

void SkeletonRenderer::sg_after(Matrix transform, SceneGraphNode * node)
{
	renderJoint(transform, node);
}

void SkeletonRenderer::renderJoint(Matrix transform, SceneGraphNode * node)
{
	JointNode* converted =(JointNode*)node;
	Matrix t_transposed = ~transform;
	Vector endPoint=Vector(0,0,0,1) * t_transposed;

	if(!converted->isRoot() && !converted->isLeaf())
	{
		NMS_DebugDraw().drawLine(startingPoint,endPoint,Vector(0,255,0,0));
	}
	NMS_DebugDraw().drawSphere(endPoint,0.1f,Vector(255,0,0),40,40);
	NMS_DebugDraw().draw3dText(endPoint,converted->getSSID().c_str());
	startingPoint = endPoint;
}

void Skeleton::addJoint(string sID,JointNode node)
{
	joints[sID]=node;
}