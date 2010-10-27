#ifdef __EXP_NMS_GL
#    define OBJECT_D __declspec(dllexport)
#else
#    define OBJECT_D __declspec(dllimport)
#endif

#ifndef NMS_3DObject_H__
#define NMS_3DObject_H__



#include <fstream>
#include "Vector.h"
#include "Trig.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>
#include "NMS_Mesh.h"

#define NOMINMAX

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 


class OBJECT_D NMS_3DObject : public NMS_Mesh
{
    public:
		NMS_3DObject();
		~NMS_3DObject();
		NMS_3DObject(const Vector* vPos,const Vector* vDir);
		Vector getPosVector();
		void   setPosVector(const Vector* vPosition);
		Vector getDirVector();
		void   setDirVector(const Vector* vDirection);
		//Print overloading
	     friend  ostream&  operator<<(ostream& output, const NMS_3DObject& obj);
		 friend bool operator == (const NMS_3DObject &a, const NMS_3DObject &b);
    private:
		Vector vPos;
		Vector vDir;
};

#endif