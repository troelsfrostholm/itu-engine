#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

class  __declspec(dllexport) NMSCameraController
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
		virtual void recalcAxes()=0;
		virtual void init()=0;
};
