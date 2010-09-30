#ifdef __EXP_NMS_GL
#    define LIGHTSYSTEM_D __declspec(dllexport)
#else
#    define LIGHTSYSTEM_D __declspec(dllimport)
#endif

#ifndef NMS_LightSystem_H__
#define NMS_LightSystem_H__

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include "NMS_3DObject.h"


class LIGHTSYSTEM_D LightSource: public NMS_3DObject
{
	public:
		GLenum     getLightNumber();
		void       setLightNumber(GLenum eNumber);
		Vector     getLightValue();
		void       setLightValue(const Vector* vValue);
		GLfloat	   getIntDis();
		void       setIntDis(GLfloat fIntDist);
		GLenum     getAttType();
		void       setAttType(GLenum eType);
		GLfloat	   getAttFact();
		void       setAttFact(GLfloat fAttFact);
		LightSource();
	private:
		Vector   vLightValue;
		GLenum   eAttType;
		GLenum   eLightNumber;
		GLfloat  fAttFactor;
		GLfloat  fIntDis;


};


class LIGHTSYSTEM_D NMS_LightSystem
{
public:
	NMS_LightSystem();
	void Enable(int bShading);
	void Disable();
	void setGlobalAmbient(const Vector* vVector);
	//It will initialize all the parameters that are not set to NULL
	void defineLight(LightSource source);
};
#endif