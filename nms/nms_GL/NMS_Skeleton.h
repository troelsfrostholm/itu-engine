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

using namespace std;


class SKELETON_D JointNode : public SceneGraphNode
{
protected:
	Matrix transform;
	string sID;
	string sName;
	string sSID;
	string sType;

public:
	JointNode();
	JointNode(Matrix t);
	void JointNode::before(SceneGraphVisitor *v, Matrix *m);
	void JointNode::after(SceneGraphVisitor *v, Matrix *m);
};

class SKELETON_D Skeleton
{
protected:
	JointNode root;
	string sID;
	string sName;
	string sSID;
	string sType;

	std::map<string,JointNode> joints;

public:
	Skeleton();
};

class SKELETON_D SkeletonRenderer : public SceneGraphVisitor
{
};


#endif