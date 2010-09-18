#ifndef NMSFramework_H__
#define NMSFramework_H__

#include "SDL.h" 
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include "Vector.h"
#include "Matrix.h"



#define NUM_PI 3.141592654f


class NMSCamera
{
	public:
	Vector position; //It stores the coordinates of the position of the camera
	
	float camYaw;
	float camPitch;
	float camRoll;

	Vector up,look,right;

	float rightVelocity;
	float upVelocity;
	float directionVelocity;

	Matrix cameraView;


	NMSCamera(){};
	void Position(float positionX,float positionY,float positionZ,
				  double pitchAngle,double yawAngle);
	void Move(float directionVelocity,float rightVelocity,float upVelocity);
	void Rotate();
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

