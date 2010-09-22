#include "NMS_CameraController.h"

class __declspec(dllexport) NMSCameraFPS : public NMS_CameraController
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