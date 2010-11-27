#include "NMS_Skeleton.h"


JointNode::JointNode() : TransformationNode() 
{
	mInverseBind = Matrix();
	mWorldMatrix = Matrix();
	mSkinningMatrix = Matrix();
	 
}

JointNode::JointNode(string sID, string sName, string sSID, string sType,Matrix t) : TransformationNode(t) 
{
	this->sID=sID;
	this->sName=sName;
	this->sSID=sSID;
	this->sType=sType;

	mInverseBind = Matrix();
	mWorldMatrix = Matrix();
	mSkinningMatrix = Matrix();
}

void JointNode::setInverseBind(Matrix m)
{
	mInverseBind=m;
}

JointNode* Skeleton::getJoint(string sID)
{
	JointNode* toBeReturned=&joints[sID];
	return toBeReturned;
}

JointNode* Skeleton::getJointsSID(string sSID)
{
	JointNode* toBeReturned=NULL;
	std::map<string,JointNode>::iterator p;
	for(p = joints.begin(); p != joints.end(); p++) 
	{
		if(p->second.getSSID()==sSID)
			toBeReturned=&p->second;
	}
	return toBeReturned;
}


string JointNode::getSSID()
{
	return sSID;
}

string JointNode::getSID()
{
	return sID;
}

Matrix JointNode::getWorldMatrix()
{
	return mWorldMatrix;
}

Matrix JointNode::getSkinningMatrix()
{
	return mSkinningMatrix;
}


JointNode* JointNode::getParent()
{
	return (JointNode*)parent;
}

void JointNode::before(SceneGraphVisitor *v, Matrix *m)
{
	
	TransformationNode::before(v, m);
	mWorldMatrix=*m;
	//Save the world matrix for the current node and precalculate the skinning matrix used in the skinning
	mSkinningMatrix=~(mWorldMatrix*mInverseBind);
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
	NMS_DebugDraw().drawSphere(endPoint,0.05f,Vector(255,0,0),5,5);
	//NMS_DebugDraw().draw3dText(endPoint,converted->getSID().c_str());
	startingPoint = endPoint;
}

void Skeleton::addJoint(string sID,JointNode node)
{
	joints[sID]=node;
}

void KeyFrame::setTime(float time)
{
	fTime=time;
}

void KeyFrame::setTransform(Matrix *m)
{
	mTransform=m;
}

float KeyFrame::getTime()
{
	return fTime;
}

Matrix* KeyFrame::getTransform()
{
	return mTransform;
}