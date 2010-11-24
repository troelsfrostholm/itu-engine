/****************************************************************************************
DLL EXPORTING SAFE
*****************************************************************************************/

/*
	Scene graph does not protect you against circular dependencies. If you make one, 
	functions like traverse_df and backtrack_to_root will make infinite loops. 
*/

#ifdef __EXP_NMS_GL
#    define SKELETON_D __declspec(dllexport)
#else
#    define SKELETON_D __declspec(dllimport)
#endif

#ifndef NMS_Skeleton_H__
#define NMS_Skeleton_H__

#include "NMS_SceneGraph.h"
#include "NMS_DebugDraw.h"
#include "Matrix.h"

using namespace std;


class SKELETON_D JointNode : public TransformationNode
{
protected:
	//Remember, the Joint Matrix for the node is saved inside "transformation" but it's just the local
	//transformation for the node.
	string sID;
	string sName;
	string sSID;
	string sType;
    Matrix     mInverseBind;  // The Inverse Bind Pose Matrix
    Matrix     mWorldMatrix;        // The World Matrix
    Matrix     mSkinningMatrix;      // The Matrix Used for calculations
	
public:
	JointNode();
	JointNode(string sID, string sName, string sSID, string sType,Matrix t);
	void JointNode::before(SceneGraphVisitor *v, Matrix *m);
	void JointNode::after(SceneGraphVisitor *v, Matrix *m);
	JointNode* getParent();
	Matrix getBindShape();
	Matrix getWorldMatrix();
	Matrix getSkinningMatrix();
	string getSSID();
	string getSID();
	void setInverseBind(Matrix m);
};

class SKELETON_D Skeleton
{
protected:
	JointNode root;

	std::map<string,JointNode> joints;

public:
	Skeleton(){};
	Skeleton(JointNode root)
	{
		this->root=root;
	};

	void addJoint(string sID,JointNode node);
	JointNode* getJoint(string sID);
	JointNode* getJointsSID(string sSID);
};

class SKELETON_D SkeletonRenderer : public SceneGraphVisitor
{
	Vector startingPoint;
	void sg_before(Matrix transform, SceneGraphNode * node);
	void sg_after(Matrix transform, SceneGraphNode * node);
public:
	SkeletonRenderer();
	void renderJoint(Matrix transform, SceneGraphNode * node);
	
};


#endif