#include <vector>
#include <iostream>
#include "Matrix.h"

using namespace std;

class  __declspec(dllexport) Visitor
{
public:
	void callback(Matrix arg);
};

class __declspec(dllexport) SceneGraphNode
{
protected:
	vector<SceneGraphNode*> children;

public:
	SceneGraphNode::SceneGraphNode();
	void traverse_df(Visitor v, Matrix *m); //depth first traversal
	void addChild(SceneGraphNode &child);
	virtual void SceneGraphNode::before(Visitor v, Matrix *m) = 0;
	virtual void SceneGraphNode::after(Visitor v, Matrix *m) = 0;
};

class __declspec(dllexport) TransformationNode : public SceneGraphNode
{
protected:
	Matrix transform;

public:
	TransformationNode::TransformationNode(Matrix t);
	void TransformationNode::before(Visitor v, Matrix *m);
	void TransformationNode::after(Visitor v, Matrix *m);
};

class __declspec(dllexport) GeometryNode : public SceneGraphNode
{

};