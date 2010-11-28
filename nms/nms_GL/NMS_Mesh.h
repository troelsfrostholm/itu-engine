#ifdef __EXP_NMS_GL
#    define NMSMESH_D __declspec(dllexport)
#else
#    define NMSMESH_D __declspec(dllimport)
#endif

#ifndef __NMS_MESH_H__
#define __NMS_MESH_H__

//#include "SDL_opengl.h"
#include "NMS_ShaderManager.h"
#include "glut.h"
#include "Vector.h"
#include "NMS_AssetManager.h"

struct NMS_Material
{
	Vector ambient;
	Vector diffuse;
	Vector specular;
	float shininess;
	string sphereMap;
	int smapTexId;
};

class NMSMESH_D NMS_Mesh
{
	public:
		NMS_Material material;

		NMS_Mesh();
		void setMaterialGL();
		virtual void render(float time) = 0;
};

class NMSMESH_D NMS_VerticalPlane : public NMS_Mesh
{
public:
	NMS_VerticalPlane(){};
	~NMS_VerticalPlane(){};
	void render(float time)
	{
		//glColor3f(0.f, 0.f, 1.f);
		float size = 20;
		glBegin(GL_QUADS);
			glVertex3f(size, 0, size);
			glVertex3f(-size, 0, size);
			glVertex3f(-size, 0, -size);
			glVertex3f(size, 0, -size);
		glEnd();
		//glColor3f(1.f, 1.f, 1.f);
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
		NMS_SHADER_MANAGER->disableTextures();

		glBegin(GL_QUADS);

			// Front Face
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(-100.0f, -100.0f,  100.0f);	// Point 1 (Front)
			glVertex3f( 100.0f, -100.0f,  100.0f);	// Point 2 (Front)
			glVertex3f( 100.0f,  100.0f,  100.0f);	// Point 3 (Front)
			glVertex3f(-100.0f,  100.0f,  100.0f);	// Point 4 (Front)
			// Back Face
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3f(-100.0f, -100.0f, -100.0f);	// Point 1 (Back)
			glVertex3f(-100.0f,  100.0f, -100.0f);	// Point 2 (Back)
			glVertex3f( 100.0f,  100.0f, -100.0f);	// Point 3 (Back)
			glVertex3f( 100.0f, -100.0f, -100.0f);	// Point 4 (Back)
			// Top Face
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-100.0f,  100.0f, -100.0f);	// Point 1 (Top)
			glVertex3f(-100.0f,  100.0f,  100.0f);	// Point 2 (Top)
			glVertex3f( 100.0f,  100.0f,  100.0f);	// Point 3 (Top)
			glVertex3f( 100.0f,  100.0f, -100.0f);	// Point 4 (Top)
			// Bottom Face
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(-100.0f, -100.0f, -100.0f);	// Point 1 (Bottom)
			glVertex3f( 100.0f, -100.0f, -100.0f);	// Point 2 (Bottom)
			glVertex3f( 100.0f, -100.0f,  100.0f);	// Point 3 (Bottom)
			glVertex3f(-100.0f, -100.0f,  100.0f);	// Point 4 (Bottom)
			// Right face
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f( 100.0f, -100.0f, -100.0f);	// Point 1 (Right)
			glVertex3f( 100.0f,  100.0f, -100.0f);	// Point 2 (Right)
			glVertex3f( 100.0f,  100.0f,  100.0f);	// Point 3 (Right)
			glVertex3f( 100.0f, -100.0f,  100.0f);	// Point 4 (Right)
			// Left Face
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3f(-100.0f, -100.0f, -100.0f);	// Point 1 (Left)
			glVertex3f(-100.0f, -100.0f,  100.0f);	// Point 2 (Left)
			glVertex3f(-100.0f,  100.0f,  100.0f);	// Point 3 (Left)
			glVertex3f(-100.0f,  100.0f, -100.0f);	// Point 4 (Left)
		
		glEnd();
		//glColor3f(1.f, 1.f, 1.f);
	}
};
	
class NMSMESH_D NMS_Sphere : public NMS_Mesh
{
public:
	void render(float time)
	{
		NMS_SHADER_MANAGER->disableTextures();
		glutSolidSphere(100, 30, 30);
	}
};

class NMSMESH_D NMS_EnvionmentMappedSphere : public NMS_Mesh
{
private:
	int textureID;

public:
	NMS_EnvionmentMappedSphere(char * filename)
	{
		textureID = NMS_ASSETMANAGER.LoadTexture(filename, filename);
	}

	void render(float time)
	{
		NMS_SHADER_MANAGER->enableTextures();
		//glBindTexture( GL_TEXTURE_2D, textureID );
		//glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glutSolidSphere(100, 10, 10);
	}
};
#endif

