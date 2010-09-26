#include "NMS_Camera.h"

//CAMERA CONTROLELR FUNCTION DEFINITIONS

void NMSCameraFPS::init()
{
	vPosition=Vector(0,0,0);
	vRight=Vector(1,0,0);
	vUp=Vector(0,1,0);
	vDir=Vector(0,0,1);
	vVel=Vector(0,0,0);

	fRotX=0;
	fRotY=0;
	fRotZ=0;

	fPitchSpd=0;
	fYawSpd=0;
	fRollSpd=0;

	qRot=Quaternion(0,0,0,1);
}


NMSCameraController::NMSCameraController()
{
}

NMSCameraController::~NMSCameraController()
{
}



Matrix NMSCameraController::returnViewMatrix()
{
	Matrix toBeReturned=Matrix();
	toBeReturned.setRow(1,vRight);
	toBeReturned.setRow(2,vUp);
	toBeReturned.setRow(3,vDir);
	Matrix trans=Matrix();
	trans.translate(vPosition);
	toBeReturned*=trans;
	toBeReturned(4,4)=1.0f;
	return toBeReturned;
}



//CAMERA FPS FUNCTION DEFINITIONS
NMSCameraFPS::NMSCameraFPS()
{
	init();
}

NMSCameraFPS::~NMSCameraFPS()
{
}

void NMSCameraFPS::setRotation(float rx, float ry, float rz)
{
	fRotX=rx;
	fRotY=ry;
	fRotZ=rz;
	recalcAxes();
}

Vector NMSCameraFPS::getRotation()
{
	return Vector(fRotX,fRotY,fRotZ);
}

void NMSCameraFPS::recalcAxes()
{
	Matrix m;

	vRight=Vector(1.0f,0.0f,0.0f);
	vUp=Vector(0.0f,1.0f,0.0f);
	vDir=Vector(0.0f,0.0f,1.0f);

	//Keep into the range of 360 degrees to avoid overflow
	//Check on Y
	if(fRotY>360.0f)
		fRotY-=360.0f;
	else
	  if(fRotY<-360.0f)
	    fRotY+=360.0f;

	if (fRotX>90.0f)
        fRotX = 90.0f;
    else if (fRotX<-90.0f)
        fRotX = -90.0f;

	//rotate around Y
	m.rotV(fRotY,vUp);
	vRight*=m;
	vDir*=m;

	//rotate around X 
	m.rotV(fRotX,vRight);
	vUp*=m;
	vDir*=m;


	//Correct rounding errors
	vDir=vDir.normal();
	vRight=vUp%vDir;
	vRight=vRight.normal();
	vUp=vDir%vRight;
	vUp=vUp.normal();
}

void NMSCameraFPS::UpdateCamera(float fET)
{
	Vector vTemp;
	fRotX+=fPitchSpd*fET;
	fRotY+=fYawSpd*fET;
	fRotZ+=fRollSpd*fET;

	vVel=vDir*fSpeed*fET;
	vTemp=vRight*fSlide*fET;
	vPosition+=vVel+vTemp;
	recalcAxes();
}