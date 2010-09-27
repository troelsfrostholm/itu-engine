#include <vector>
#include <iostream>
#include "Matrix.h"
#include "MD2Model.h"

using namespace std;

class __declspec(dllexport) Mesh
{
public:
	void render();
};

class  __declspec(dllexport) SceneGraphVisitor
{
public:
	virtual void sg_before(Matrix transform, Mesh model) = 0;
	virtual void sg_after(Matrix transform, Mesh model) = 0;
};

class __declspec(dllexport) SceneGraphNode
{
protected:
	vector<SceneGraphNode*> children;

public:
	SceneGraphNode::SceneGraphNode();
	void traverse_df(SceneGraphVisitor *v);            //depth first traversal, starting with identity matrix
	void traverse_df(SceneGraphVisitor *v, Matrix *m); //depth first traversal, starting with matrix m
	void addChild(SceneGraphNode* child);
	virtual void SceneGraphNode::before(SceneGraphVisitor *v, Matrix *m) = 0;
	virtual void SceneGraphNode::after(SceneGraphVisitor *v, Matrix *m) = 0;
};

class __declspec(dllexport) TransformationNode : public SceneGraphNode
{
protected:
	Matrix transform;

public:
	TransformationNode::TransformationNode(Matrix t);
	void TransformationNode::before(SceneGraphVisitor *v, Matrix *m);
	void TransformationNode::after(SceneGraphVisitor *v, Matrix *m);
};

class __declspec(dllexport) GeometryNode : public SceneGraphNode
{
protected:
	Mesh *model;

public:
	GeometryNode::GeometryNode(Mesh *m);
	void GeometryNode::before(SceneGraphVisitor *v, Matrix *m);
	void GeometryNode::after(SceneGraphVisitor *v, Matrix *m);
};