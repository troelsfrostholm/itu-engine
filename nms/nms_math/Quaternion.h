#ifndef QUATERNION_H__
#define QUATERNION_H__

#include <fstream>
#include <cmath>
#include "Vector.h"
#include "Matrix.h"

using namespace std;

//Three space vector class
class __declspec(dllexport) Quaternion : public Vector{
public:
	Quaternion(void) {x=0.0f, y=0.0f, z=0.0f, w=1.0f;}
	Quaternion(float x,float y,float z,float w)
	{this->x=x; this->y=y; this->z=z; this->w=w;}

	Matrix getMatrix();

	void createFromAngles(float fPitch, float fYaw, float fRoll);

	Quaternion operator * (const Quaternion &q)const;
	void operator *= (const Quaternion &q);
};
#endif