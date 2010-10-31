#ifdef __EXP_NMS_GL
#    define DEBUGDRAW_D __declspec(dllexport)
#else
#    define DEBUGDRAW_D __declspec(dllimport)
#endif

#ifndef __NMS_DEBUGDRAW
#define __NMS_DEBUGDRAW

#include "SDL_opengl.h"
#include "Vector.h"
#include "glut.h"

class DEBUGDRAW_D NMS_DebugDraw
{
	public:
		void drawLine(const Vector& from,const Vector& to,const Vector& fromColor, const Vector& toColor);
		void drawLine(const Vector& from,const Vector& to,const Vector& color);
		void drawSphere(const Vector& p, float radius, const Vector& color,int lats,int longs);
		void drawBox(const Vector& boxMin, const Vector& boxMax, const Vector& color, int alpha);
		void drawTriangle(const Vector& a,const Vector& b,const Vector& c,const Vector& color,int alpha);
		void draw3dText(const Vector& location,const char* textString);
		void setDebugMode(int debugMode);
		int getDebugMode();
};
#endif