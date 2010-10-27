#ifdef __EXP_NMS_GL
#    define NMSMESH_D __declspec(dllexport)
#else
#    define NMSMESH_D __declspec(dllimport)
#endif

#ifndef __NMS_MESH_H__
#define __NMS_MESH_H__

#define NOMINMAX

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 


class NMSMESH_D NMS_Mesh
{
	public:
		NMS_Mesh(){};
		~NMS_Mesh(){};
		virtual void render(float time) = 0;
};

class NMSMESH_D NMS_VerticalPlane : public NMS_Mesh
{
public:
	NMS_VerticalPlane(){};
	~NMS_VerticalPlane(){};
	void render(float time)
	{
		glColor3f(0.f, 0.f, 1.f);
		float size = 20;
		glBegin(GL_QUADS);
			glVertex3f(size, 0, size);
			glVertex3f(-size, 0, size);
			glVertex3f(-size, 0, -size);
			glVertex3f(size, 0, -size);
		glEnd();
		glColor3f(1.f, 1.f, 1.f);
	}
};

class NMSMESH_D NMS_Plane : public NMS_Mesh
{
public:
	void render(float time)
	{
		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_QUADS);
			glVertex3f(-1000,0,1000);
			glVertex3f(-1000,0,-1000);
			glVertex3f(1000,0,-1000);
			glVertex3f(1000,0,1000);
		glEnd();
		glColor3f(1.f, 1.f, 1.f);
	}
};

class NMSMESH_D NMS_Cube : public NMS_Mesh
{
public:
	void render(float time)
	{
	glColor3f(0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
		// Front Face
		glVertex3f(-100.0f, -100.0f,  100.0f);	// Point 1 (Front)
		glVertex3f( 100.0f, -100.0f,  100.0f);	// Point 2 (Front)
		glVertex3f( 100.0f,  100.0f,  100.0f);	// Point 3 (Front)
		glVertex3f(-100.0f,  100.0f,  100.0f);	// Point 4 (Front)
		// Back Face
		glVertex3f(-100.0f, -100.0f, -100.0f);	// Point 1 (Back)
		glVertex3f(-100.0f,  100.0f, -100.0f);	// Point 2 (Back)
		glVertex3f( 100.0f,  100.0f, -100.0f);	// Point 3 (Back)
		glVertex3f( 100.0f, -100.0f, -100.0f);	// Point 4 (Back)
		// Top Face
		glVertex3f(-100.0f,  100.0f, -100.0f);	// Point 1 (Top)
		glVertex3f(-100.0f,  100.0f,  100.0f);	// Point 2 (Top)
		glVertex3f( 100.0f,  100.0f,  100.0f);	// Point 3 (Top)
		glVertex3f( 100.0f,  100.0f, -100.0f);	// Point 4 (Top)
		// Bottom Face
		glVertex3f(-100.0f, -100.0f, -100.0f);	// Point 1 (Bottom)
		glVertex3f( 100.0f, -100.0f, -100.0f);	// Point 2 (Bottom)
		glVertex3f( 100.0f, -100.0f,  100.0f);	// Point 3 (Bottom)
		glVertex3f(-100.0f, -100.0f,  100.0f);	// Point 4 (Bottom)
		// Right face
		glVertex3f( 100.0f, -100.0f, -100.0f);	// Point 1 (Right)
		glVertex3f( 100.0f,  100.0f, -100.0f);	// Point 2 (Right)
		glVertex3f( 100.0f,  100.0f,  100.0f);	// Point 3 (Right)
		glVertex3f( 100.0f, -100.0f,  100.0f);	// Point 4 (Right)
		// Left Face
		glVertex3f(-100.0f, -100.0f, -100.0f);	// Point 1 (Left)
		glVertex3f(-100.0f, -100.0f,  100.0f);	// Point 2 (Left)
		glVertex3f(-100.0f,  100.0f,  100.0f);	// Point 3 (Left)
		glVertex3f(-100.0f,  100.0f, -100.0f);	// Point 4 (Left)
	glEnd();
	glColor3f(1.f, 1.f, 1.f);
	}
};

#endif

