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
#include "Vector.h"



class LIGHTSYSTEM_D AmbientLight: public NMS_3DObject
{
	public:
		AmbientLight();
		void       setGlobalAmbient(const Vector* vVector);
		Vector	   getGlobalAmbient();
		void	   render(float time);
	private:
		GLfloat	   aLightValue[4];
		bool       bToBeUpdated;
};


class LIGHTSYSTEM_D LightSource: public NMS_3DObject
{
	public:
		LightSource();
		void	   defineLight(LightSource source);
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
		void	   render(float time);
	private:
		Vector     vLightValue;
		GLenum     eAttType;
		GLenum     eLightNumber;
		GLfloat    fAttFactor;
		GLfloat    fIntDis;
		bool       bToBeUpdated;
};


class LIGHTSYSTEM_D NMS_LightSystem
{
public:
	NMS_LightSystem();
	void Enable(int bShading);
	void Disable();
	//It will initialize all the parameters that are not set to NULL
};
#endif