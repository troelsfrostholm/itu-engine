#include "NMS_Skeleton.h"


JointNode::JointNode()
{
}

JointNode::JointNode(string sID, string sName, string sSID, string sType,Matrix t)
{
	this->sID=sID;
	this->sName=sName;
	this->sSID=sSID;
	this->sType=sType;
	this->transform=t;
}


string JointNode::getSSID()
{
	return sSID;
}

void JointNode::before(SceneGraphVisitor *v, Matrix *m)
{
	JointNode::before(v, m);
	v->sg_before(*m, this);
}

void JointNode::after(SceneGraphVisitor *v, Matrix *m) 
{
	v->sg_after(*m, this);
	JointNode::after(v, m);
}



void SkeletonRenderer::sg_before(Matrix transform, JointNode * node)
{
	glLoadIdentity();
	Matrix t_transposed = ~transform;
	glMultMatrixf(t_transposed.returnPointer());
	NMS_DebugDraw().drawSphere(Vector(0,0,0),3,Vector(255,0,0),5,5);
	NMS_DebugDraw().draw3dText(Vector(0,0,0),node->getSSID().c_str());
}

void SkeletonRenderer::sg_after(Matrix transform, JointNode * node)
{}

void Skeleton::addJoint(string sID,JointNode node)
{
}