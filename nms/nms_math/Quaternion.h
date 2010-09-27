#define QUATERNION_D __declspec(dllexport)


#ifndef QUATERNION_H__
#define QUATERNION_H__

#include <fstream>
#include <cmath>
#include "Vector.h"
#include "Matrix.h"

using namespace std;

//Three space vector class
class QUATERNION_D Quaternion : public Vector{
public:
	Quaternion(void);
	Quaternion(float x,float y,float z,float w);

	Matrix getMatrix();

	void createFromAngles(float fPitch, float fYaw, float fRoll);

	Quaternion operator * (const Quaternion &q)const;
	void operator *= (const Quaternion &q);
};
#endif