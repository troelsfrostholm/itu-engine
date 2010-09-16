#ifndef VECTOR_H__
#define VECTOR_H__

#include <fstream>
#include <cmath>
#include "Point.h"
//#include "Matrix.h"

using namespace std;

//Three space vector class
class Vector : public Point{
  
  public:
	  //CONSTRUCTORS
	   __declspec(dllexport) Vector();
	   __declspec(dllexport) Vector(float x,float y,float z);
	   __declspec(dllexport) Vector(float x,float y,float z,float w);



	  //OPERATORS
	   __declspec(dllexport) Vector&  Vector::operator+=(Vector const &v);
	   __declspec(dllexport) Vector&  Vector::operator-=(Vector const &v);


	   __declspec(dllexport) Vector&  Vector::operator*=(float f);
	   __declspec(dllexport) friend   Vector operator *(Vector& v,const Matrix &m);
	   __declspec(dllexport) Vector&  operator *= (const Matrix &m);


	   __declspec(dllexport) Vector& Vector::operator/=(float f);
	   __declspec(dllexport) Vector  Vector::operator*(float f);
	   __declspec(dllexport) Vector  Vector::operator+(Vector const &v);
	   __declspec(dllexport) Vector  Vector::operator-(Vector const &v);
	  //Dot Product
	   __declspec(dllexport) float   Vector::operator*(Vector const &v);
	  //Cross Product
	   __declspec(dllexport) Vector  Vector::operator%(Vector const &v);

	   __declspec(dllexport) Vector  Vector::normal();
	   __declspec(dllexport) float   Vector::magnitude();
	   __declspec(dllexport) float   Vector::sqmagnitude();

	   __declspec(dllexport) friend ostream& operator<<(ostream& output, const Vector& v);
};
#endif