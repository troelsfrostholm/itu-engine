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


class KeyFrame
{
public:
	 KeyFrame(){};
	 ~KeyFrame(){};
	 void setTime(float time);
	 void setTransform(Matrix *m);
	 float getTime();
	 Matrix* getTransform();
private:
     float          fTime;                // Time of this key frame
     Matrix         mTransform;          // Transformation of this Key frame
};

class SKELETON_D JointNode : public TransformationNode
{
protected:
	//Remember, the Joint Matrix for the node is saved inside "transformation" but it's just the local
	//transformation for the node. To get the world matrix you have to get it inside the scenegraph of 
	//the skeleton, in which is calculated while traversing it
	string sID;
	string sName;
	string sSID;
	string sType;
    Matrix     mInverseBind;			   // The Inverse Bind Pose Matrix
    Matrix     mWorldMatrix;			   // The World Matrix
    Matrix     mSkinningMatrix;			   // The Matrix Used for calculations
	unsigned   iNKeyFrames;			   // Number of Keyframes used by this joint
    KeyFrame   *pAnimationFrames;         // All Key frames for this Bone’s animation
	float      fCurrentTime;
	unsigned   iCurrentFrame;
	unsigned   iFps;
	
	void Animate(float time,Matrix *m);
	Matrix LERP(Matrix *current,Matrix *next,float beta);

public:
	JointNode();
	JointNode(string sID, string sName, string sSID, string sType,Matrix t);
	void JointNode::before(SceneGraphVisitor *v, Matrix *m);
	void JointNode::after(SceneGraphVisitor *v, Matrix *m);
	JointNode* getParent();
	Matrix getWorldMatrix();
	Matrix getSkinningMatrix();
	string getSSID();
	string getSID();
	void setInverseBind(Matrix m);
	void initializeKeyframes(unsigned size);
	void setKeyFrame(KeyFrame k,unsigned position);
	unsigned getNKeyFrames();
};

class SKELETON_D Skeleton : public NMS_Mesh
{
protected:
	JointNode root;
	string rootNode;
	std::map<string,JointNode> joints;

public:
	Skeleton(){};
	Skeleton(JointNode root,string rootNode)
	{
		this->root=root;
		this->rootNode=rootNode;
	};
	void	render(float time);
	void addJoint(string sID,JointNode node);
	JointNode* getJoint(string sID);
	JointNode* getJointsSID(string sSID);
};

class SKELETON_D SkeletonRenderer : public SceneGraphVisitor
{
	Vector startingPoint;
	float  fAnimationTime;
	void sg_before(Matrix transform, SceneGraphNode * node);
	void sg_after(Matrix transform, SceneGraphNode * node);
public:
	SkeletonRenderer();
	void renderJoint(Matrix transform, SceneGraphNode * node);
	void setAnimationTime(float time);
	float getAnimationTime();
};

#endif