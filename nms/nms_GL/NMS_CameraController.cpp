#include "NMS_CameraController.h"

//CAMERA CONTROLELR FUNCTION DEFINITIONS
NMS_CameraController::NMS_CameraController()
{
	
}

NMS_CameraController::~NMS_CameraController()
{
}

Matrix NMS_CameraController::returnViewMatrix()
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