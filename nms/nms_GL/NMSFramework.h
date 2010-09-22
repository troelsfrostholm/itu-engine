#ifndef NMSFramework_H__
#define NMSFramework_H__

#include "SDL.h" 
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Trig.h"

class NMSCameraController
{
	public:
		NMSCameraController();
		~NMSCameraController();

		void UpdateCamera(float fElapsedTime);

		//Access methods
		Vector getPos() {return vPosition;}
		Vector getRight() {return vRight;}
		Vector getUp() {return vUp;}
		Vector getDir() {return vDir;}
		Vector getVelocity() {return vVel;}
		Matrix returnViewMatrix();

	protected:

		Vector vPosition;  //Position vector of the camera
		Vector vRight;	   //Right vector of the camera
		Vector vUp;		   //Up vector of the camera
		Vector vDir;	   //Direction vector of the camera
		Vector vVel;	   //Velocity vector of the camera
		Quaternion qRot;   //Quaternion used for the rotations
		
		//Rotation speed with respect to the local axes
		float fRollSpd;
		float fPitchSpd;
		float fYawSpd;

		//Current rotation angle on the axis
		float fRotX;
		float fRotY;
		float fRotZ;

		//Protected methods
		virtual void recalcAxes();
		virtual void init();
};






class NMSCameraFPS : public NMSCameraController
{
	public:
		NMSCameraFPS();
		~NMSCameraFPS();

		Vector getRotation();
		void UpdateCamera(float fET);

		void setRSpeedX(float f){fPitchSpd=f;}
		void setRSpeedY(float f){fYawSpd=f;}
		float getRSpeedX(){return fPitchSpd;}
		float getRSpeedY(){return fYawSpd;}
		
		void setSpeed(float f){fSpeed=f;}
		void setSlideSpeed(float f){fSlide=f;}
		float getSpeed(){return fSpeed;}
		float getSlideSpeed(){return fSlide;}

		void setRotation(float rx, float ry, float rz);
		void setPos(Vector &v){vPosition=v;}
		void setRight(Vector &v){vRight=v;}
		void setUp(Vector &v){vUp=v;}
		void setDir(Vector &v){vDir=v;}

private:
	float fSpeed;
	float fSlide;
	void recalcAxes();
};


class NMSFramework
{
protected:
	int flags;
	
public:
	bool running;
	NMSCameraFPS camera;
	NMSFramework();
	bool NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen);
	void NMSQuit();
	static void NMSLoadTexture(char* fileName,int id);
	void CalculateFrameRate();
};
#endif;

