#ifndef NMSFramework_H__
#define NMSFramework_H__

#include "SDL.h" 
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include "Vector.h"
#include "Matrix.h"

#define view 3
#define up 2

class NMSCamera
{
	public:
	Matrix mPosition; // posizione della camera
	Matrix mView;
	Matrix mUp;
	
	float rotRight;
	float rotUp;

	float rightVelocity;
	float upVelocity;
	float directionVelocity;


	NMSCamera(){};
	void Position(float positionX,float positionY,float positionZ,
				  double upAngle,double viewAngle);
	void Move(float directionVelocity,float rightVelocity,float upVelocity);
	void Rotate(float rotX,float rotY);
	void updatePositions();
};


class NMSFramework
{
protected:
	int flags;
	
public:
	NMSCamera camera;
	bool running;
	NMSFramework();
	bool NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen);
	void NMSQuit();
	void NMSLoadTexture(char* fileName,int id);
	void CalculateFrameRate();
};
#endif;

