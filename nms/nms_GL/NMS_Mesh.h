#ifdef __EXP_NMS_GL
#    define NMSMESH_D __declspec(dllexport)
#else
#    define NMSMESH_D __declspec(dllimport)
#endif

#ifndef __NMS_MESH_H__
#define __NMS_MESH_H__

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

#endif