#ifdef __EXP_NMS_GL
#    define CAMERA_D __declspec(dllexport)
#else
#    define CAMERA_D __declspec(dllimport)
#endif

#ifndef NMS_CAMERA_H
#define NMS_CAMERA_H

#include "Matrix.h"

class  CAMERA_D NMSCameraController
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
		
		//Rotation speed with respect to the local axes
		float fRollSpd;
		float fPitchSpd;
		float fYawSpd;

		//Current rotation angle on the axis
		float fRotX;
		float fRotY;
		float fRotZ;

		//Protected methods
		virtual void recalcAxes()=0;
		virtual void init()=0;
};

class CAMERA_D NMSCameraFPS : public NMSCameraController
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

protected:
	void recalcAxes();
	void init();

private:
	float fSpeed;
	float fSlide;
};

#endif