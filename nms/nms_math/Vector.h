#define VECTOR_D __declspec(dllexport)


#ifndef VECTOR_H__
#define VECTOR_H__

#include <fstream>
#include <cmath>
#include "Point.h"

using namespace std;

//Three space vector class
class  VECTOR_D Vector : public Point{
 // friend class Quaternion;
  public:
	  //CONSTRUCTORS
	    Vector();
	    Vector(float x,float y,float z);
	    Vector(float x,float y,float z,float w);

	  //OPERATORS
	    Vector&  Vector::operator+=(Vector const &v);
	    Vector&  Vector::operator-=(Vector const &v);


	    Vector&  Vector::operator*=(float f);
	    friend    Vector operator *(Vector& v,const Matrix &m);
	    Vector&  operator *= (const Matrix &m);


	    Vector& Vector::operator/=(float f);
	    friend  Vector operator*(Vector const &v, float f);
	    friend  Vector operator*(float f, Vector const &v);
	    friend  Vector operator+(Vector const &a, Vector const &b);
	    friend  Vector operator-(Vector const &a, Vector const &b);
	  //Dot Product
	    float   Vector::operator*(Vector const &v);
	  //Cross Product
	    Vector  Vector::operator%(Vector const &v);

	    Vector  Vector::normal();
	    float   Vector::magnitude();
	    float   Vector::sqmagnitude();
		void Vector::floatArray(float * dest);
};
#endif