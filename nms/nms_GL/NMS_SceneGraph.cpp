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

TransformationNode::TransformationNode(Matrix t)
{
	cout << "t is " << t << endl;
	transform = t;
	cout << "transform is " << transform << endl;
}

void TransformationNode::before(SceneGraphVisitor *v, Matrix *m)
{
	*m *= transform;
}

void TransformationNode::after(SceneGraphVisitor *v, Matrix *m)
{
	*m *= (!transform);
}

GeometryNode::GeometryNode(Mesh *m, btRigidBody *b)
{
	model = m;
	collisionBody = b;
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
	glColor3f(.3f,.3f,1.0f);
	glTranslatef(0.0f, 0.0f, -5.0f);
	glBegin(GL_QUADS);
		// Front Face
		glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
		glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
		glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
		glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)
		// Back Face
		glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Back)
		glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 2 (Back)
		glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 3 (Back)
		glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 4 (Back)
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

void applyPhysics(Matrix *m, btRigidBody *b)
{
	btScalar matrix[16];
	btTransform trans;
	glPushMatrix();
	b->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(matrix);
	glMultMatrixf(matrix);
	glPopMatrix();
}