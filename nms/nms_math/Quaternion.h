#define QUATERNION_D __declspec(dllexport)


#ifndef QUATERNION_H__
#define QUATERNION_H__

#include <fstream>
#include <cmath>
#include "Vector.h"
#include "Matrix.h"

using namespace std;

//Definitions and helpful conversion functions taken from here: http://web.archive.org/web/20041029003853/http:/www.j3d.org/matrix_faq/matrfaq_latest.html#Q49

class QUATERNION_D Quaternion : public Vector{
public:
	Quaternion(void);
	Quaternion(float x,float y,float z,float w);

	Matrix getMatrix();

	Quaternion createFromAngles(float fPitch, float fYaw, float fRoll);
	Quaternion createFromVector(Vector v,float angle);

	Quaternion operator *  (const Quaternion &q)const;
	Vector     operator *  (const Vector &v    )const;
	void operator       *= (const Quaternion &q);

	Quaternion          conjugate()const;
	Quaternion          normal();
	float               lenght();
	
};
#endif