#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{

}

void SceneGraphNode::addChild(SceneGraphNode* child)
{
	children.push_back(child);
}

void SceneGraphNode::traverse_df(SceneGraphVisitor *v)
{
	Matrix m = Matrix();
	traverse_df(v, &m);
}

void SceneGraphNode::traverse_df(SceneGraphVisitor *v, Matrix *m)
{
	before(v, m);
	for(unsigned int i = 0; i < children.size(); i++) {
		children[i]->traverse_df(v, m);
	}
	after(v, m);
}

TransformationNode::TransformationNode(Matrix t)
{
	transform = t;
}

void TransformationNode::before(SceneGraphVisitor *v, Matrix *m)
{
	*m *= transform;
}

void TransformationNode::after(SceneGraphVisitor *v, Matrix *m)
{
	*m *= (!transform);
}

GeometryNode::GeometryNode(Mesh *m)
{
	model = m;
}

void GeometryNode::before(SceneGraphVisitor *v, Matrix *m)
{
	v->sg_before(*m, *model);
}

void GeometryNode::after(SceneGraphVisitor *v, Matrix *m) 
{
	v->sg_after(*m, *model);
}

void Mesh::render()
{
	cout << "rendering";
	glColor3f(0.3f,0.3f,0.3f);
	glTranslatef(0.f, 0.f, -10.f);
	glBegin(GL_QUADS);
	// Front Face
		glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
		glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
		glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
		glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)
	glEnd();
}