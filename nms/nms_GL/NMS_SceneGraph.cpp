#include "NMS_SceneGraph.h"

SceneGraphNode::SceneGraphNode()
{
	children = vector<SceneGraphNode*>();
	parent = NULL;
}

void SceneGraphNode::addChild(SceneGraphNode* child)
{
	children.push_back(child);
	child->setParent(this);
}

void SceneGraphNode::setParent(SceneGraphNode* _parent)
{
	parent = _parent;
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

void SceneGraphNode::backtrack_to_root(SceneGraphVisitor *v, Matrix *m)
{
	if(!parent)  {  //This is the root node
		return;
	}
	after(v, m);
	parent->backtrack_to_root(v, m);
}

TransformationNode::TransformationNode() : SceneGraphNode()
{
}

TransformationNode::TransformationNode(Matrix t)
{
	transform = t;
}

void TransformationNode::multiply(Matrix m)
{
	transform = transform * m;
}

void TransformationNode::before(SceneGraphVisitor *v, Matrix *m)
{
	*m *= transform;
}

void TransformationNode::after(SceneGraphVisitor *v, Matrix *m)
{
	*m *= !transform;
}

GeometryNode::GeometryNode() : TransformationNode()
{ 
	model = NULL; 
}

GeometryNode::GeometryNode(NMS_Mesh *m, btRigidBody *b) : TransformationNode()
{
	model = m;
	collisionBody = b;
}

GeometryNode::GeometryNode(NMS_Mesh *m, btRigidBody *b, Matrix t) : TransformationNode(t) 
{ 
	model = m;
	collisionBody = b;
}

void GeometryNode::before(SceneGraphVisitor *v, Matrix *m)
{
	TransformationNode::before(v, m);
	v->sg_before(*m, model, collisionBody);
}

void GeometryNode::after(SceneGraphVisitor *v, Matrix *m) 
{
	v->sg_after(*m, model);
}

void NMS_Mesh::render(float time)
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