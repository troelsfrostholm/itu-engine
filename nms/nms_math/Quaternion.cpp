#include "Quaternion.h"


Quaternion::Quaternion() 
{
	x=0.0f, y=0.0f, z=0.0f, w=1.0f;
}

Quaternion::Quaternion(float x,float y,float z,float w)
{
	this->x=x; this->y=y; this->z=z; this->w=w;
}

void Quaternion::operator *=(const Quaternion &q)
{
	float tempX,tempY,tempZ,tempW;

	tempW=w*q.w-x*q.x-y*q.y-z*q.z;
	tempX=w*q.x+x*q.w+y*q.z-z*q.y;
	tempY=w*q.y+y*q.w+z*q.x-x*q.z;
	tempZ=w*q.z+z*q.w+x*q.y-y*q.x;

	x=tempX;
	y=tempY;
	z=tempZ;
	w=tempW;
}

Quaternion Quaternion::operator *(const Quaternion &q)const
{
	Quaternion toBeReturned;

	toBeReturned.w=w*q.w-x*q.x-y*q.y-z*q.z;
	toBeReturned.x=w*q.x+x*q.w+y*q.z-z*q.y;
	toBeReturned.y=w*q.y+y*q.w+z*q.x-x*q.z;
	toBeReturned.z=w*q.z+z*q.w+x*q.y-y*q.x;

	return toBeReturned;
}




Vector     Quaternion::operator *  (const Vector &v    )const
{
	Vector vObtained = v;
	vObtained.normal();
 
	Quaternion vecQuat, resQuat;
	vecQuat.x = vObtained[NMS_X];
	vecQuat.y = vObtained[NMS_Y];
	vecQuat.z = vObtained[NMS_Z];
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * conjugate();
	resQuat = *this * resQuat;
 
	return (Vector(resQuat.x, resQuat.y, resQuat.z));
}


Quaternion Quaternion::conjugate()const
{
	return Quaternion(-x,-y,-z,w);
}


void Quaternion::createFromVector(Vector v,float angle)
{
	v=v.normal();
	angle=angle/2;
	float sinAngle=nmsTrig::sin(angle);
	x = v[NMS_X] * sinAngle;
	y = v[NMS_Y] * sinAngle;
	z = v[NMS_Z] * sinAngle;
	w = nmsTrig::cos(angle);
}

void Quaternion::createFromAngles(float fPitch, float fYaw, float fRoll)
{
	float cX,cY,cZ,sX,sY,sZ,cYcZ,sYsZ,cYsZ,sYcZ;
	//Conversion from degree to radians
	fPitch= nmsTrig::degToRad(fPitch);
	fYaw=nmsTrig::degToRad(fYaw);
	fRoll=nmsTrig::degToRad(fRoll);


	fPitch *=0.5f;
	fYaw *=0.5f;
	fRoll *=0.5f;

	cX=cosf(fPitch);
	cY=cosf(fYaw);
	cZ=cosf(fRoll);

	sX=sinf(fPitch);
	sY=sinf(fYaw);
	sZ=sinf(fRoll);
	
	cYcZ=cY*cZ;
	sYsZ=sY*sZ;
	cYsZ=cY*sZ;
	sYcZ=sY*cZ;

	w=cX*cYcZ+sX*sYsZ;
	x=sX*cYcZ-cX*sYsZ;
	y=cX*sYcZ+sX*cYsZ;
	z=cX*cYsZ-sX*sYcZ;
}

Matrix Quaternion::getMatrix()
{
	float wx,wy,wz,xx,yy,yz,xy,xz,zz,x2,y2,z2;

	Matrix toBeReturned=Matrix();

	x2=x+x;
	y2=y+y;
	z2=z+z;

	xx=x*x2;
	xy=x*y2;
	xz=x*z2;

	yy=y*y2;
	yz=y*z2;
	zz=z*z2;

	wx=w*x2;
	wy=w*y2;
	wz=w*z2;

	toBeReturned(1,1)=1.0f-(yy+zz);
	toBeReturned(1,2)=xy-wz;
	toBeReturned(1,3)=xz+wy;

	toBeReturned(2,1)=xy+wz;
	toBeReturned(2,2)=1.0f-(xx+zz);
	toBeReturned(2,3)=yz-wx;

	toBeReturned(3,1)=xz-wy;
	toBeReturned(3,2)=yz+wx;
	toBeReturned(3,3)=1.0f-(xx+yy);

	return toBeReturned;
}


Quaternion Quaternion::normal()
{
	Quaternion q=(*this);
	float lenght=this->lenght();
	q.x /= lenght;
	q.y /= lenght;
	q.z /= lenght;
	q.w /= lenght;
	return q;

}


float Quaternion::lenght()
{
	return sqrt(x * x + y * y +
              z * z + w * w);
}