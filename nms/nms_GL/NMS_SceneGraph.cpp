#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{
	children = vector<SceneGraphNode*>();
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

TransformationNode::TransformationNode() : SceneGraphNode()
{
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
	glBegin(GL_QUADS);
		// Front Face
		glColor3f(.1f,.1f,0.6f);
		glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
		glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
		glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
		glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)
		// Back Face
		glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Back)
		glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 2 (Back)
		glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 3 (Back)
		glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 4 (Back)
		glColor3f(.3f,.3f,1.0f);
		// Top Face
		glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 1 (Top)
		glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 2 (Top)
		glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Top)
		glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 4 (Top)
		// Bottom Face
		glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Bottom)
		glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 2 (Bottom)
		glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 3 (Bottom)
		glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 4 (Bottom)
		// Right face
		glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 1 (Right)
		glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 2 (Right)
		glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Right)
		glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 4 (Right)
		// Left Face
		glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Left)
		glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 2 (Left)
		glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 3 (Left)
		glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 4 (Left)
	glEnd();
//	cout << "rendering";
}