#include <vector>
#include <iostream>
#include "Matrix.h"

using namespace std;

class  __declspec(dllexport) SceneGraphVisitor
{
public:
	void sg_before(Matrix arg);
	void sg_after(Matrix arg);
};

class __declspec(dllexport) SceneGraphNode
{
protected:
	vector<SceneGraphNode*> children;

public:
	SceneGraphNode::SceneGraphNode();
	void traverse_df(SceneGraphVisitor v, Matrix *m); //depth first traversal
	void addChild(SceneGraphNode &child);
	virtual void SceneGraphNode::before(SceneGraphVisitor v, Matrix *m) = 0;
	virtual void SceneGraphNode::after(SceneGraphVisitor v, Matrix *m) = 0;
};

class __declspec(dllexport) TransformationNode : public SceneGraphNode
{
protected:
	Matrix transform;

public:
	TransformationNode::TransformationNode(Matrix t);
	void TransformationNode::before(SceneGraphVisitor v, Matrix *m);
	void TransformationNode::after(SceneGraphVisitor v, Matrix *m);
};

class __declspec(dllexport) GeometryNode : public SceneGraphNode
{

public:
	void GeometryNode::before(SceneGraphVisitor v, Matrix *m);
	void GeometryNode::after(SceneGraphVisitor v, Matrix *m);
};