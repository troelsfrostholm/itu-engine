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
	  Vector();
	  Vector(float x,float y,float z);
	  Vector(float x,float y,float z,float w);



	  //OPERATORS
	  Vector&  Vector::operator+=(Vector const &v);
	  Vector&  Vector::operator-=(Vector const &v);


	  Vector&  Vector::operator*=(float f);
	  friend   Vector operator *(Vector& v,const Matrix &m);
	  Vector&  operator *= (const Matrix &m);


	  Vector& Vector::operator/=(float f);
	  Vector  Vector::operator*(float f);
	  Vector  Vector::operator+(Vector const &v);
	  Vector  Vector::operator-(Vector const &v);
	  //Dot Product
	  float   Vector::operator*(Vector const &v);
	  //Cross Product
	  Vector  Vector::operator%(Vector const &v);

	  Vector  Vector::normal();
	  float   Vector::magnitude();
	  float   Vector::sqmagnitude();

	  friend ostream& operator<<(ostream& output,Vector& v);
};
#endif