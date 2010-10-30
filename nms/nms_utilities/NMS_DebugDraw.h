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

static class DEBUGDRAW_D NMS_DebugDraw
{
	public:
		static void drawLine(const Vector& from,const Vector& to,const Vector& fromColor, const Vector& toColor);
		static void drawLine(const Vector& from,const Vector& to,const Vector& color);
		static void drawSphere(const Vector& p, int radius, const Vector& color,int lats,int longs);
		static void drawBox(const Vector& boxMin, const Vector& boxMax, const Vector& color, int alpha);
		static void drawTriangle(const Vector& a,const Vector& b,const Vector& c,const Vector& color,int alpha);
		static void draw3dText(const Vector& location,const char* textString);
		static void setDebugMode(int debugMode);
		static int getDebugMode();
};
#endif