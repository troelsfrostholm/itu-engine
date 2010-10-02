#ifdef __EXP_NMS_GL
#    define SCENEGRAPH_D __declspec(dllexport)
#else
#    define SCENEGRAPH_D __declspec(dllimport)
#endif

#ifndef NMS_Scenegraph_H__
#define NMS_Scenegraph_H__

#include <vector>
#include <iostream>
#include "Matrix.h"
#include "MD2Model.h"
#include "NMS_Mutex.h"
#include "NMS_Mesh.h"

using namespace std;

class  SCENEGRAPH_D SceneGraphVisitor
{
public:
	virtual void sg_before(Matrix transform, NMS_Mesh* model) = 0;
	virtual void sg_after(Matrix transform, NMS_Mesh* model) = 0;
};

class SCENEGRAPH_D SceneGraphNode
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

class SCENEGRAPH_D TransformationNode : public SceneGraphNode
{
protected:
	Matrix transform;

public:
	TransformationNode::TransformationNode();
	TransformationNode::TransformationNode(Matrix t);
	void TransformationNode::multiply(Matrix m);
	void TransformationNode::before(SceneGraphVisitor *v, Matrix *m);
	void TransformationNode::after(SceneGraphVisitor *v, Matrix *m);
};

class SCENEGRAPH_D GeometryNode : public SceneGraphNode
{
protected:
	NMS_Mesh *model;

public:
	GeometryNode::GeometryNode(NMS_Mesh *m);
	void GeometryNode::before(SceneGraphVisitor *v, Matrix *m);
	void GeometryNode::after(SceneGraphVisitor *v, Matrix *m);
};
#endif