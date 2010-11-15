#define Matrix4x4_D __declspec(dllexport)


#ifndef Matrix4x4_H__
#define Matrix4x4_H__

#define NOMINMAX
#include <fstream>
#include "Vector.h"
#include "Trig.h"
#include <cmath>
#include <math.h>
#include <float.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Quaternion.h"
#include <algorithm>

using namespace std;

class Matrix4x4_D Matrix4x4{
private:
	   float elements[16];

  public:
	  //CONSTRUCTORS & destructor
	     Matrix4x4::Matrix4x4();		           //Creates identity matrix
	     Matrix4x4::Matrix4x4(float* _elements);

		 const float* Matrix4x4::getElements() const;             //return matrix elements as float array

	  //Access to the Matrix4x4 values, starting from 1 NOT 0
	     float& Matrix4x4::operator()(unsigned row,unsigned col);		//Subscript access to the Matrix4x4, non const
	     float const Matrix4x4::operator()(unsigned row,unsigned col) const;	//Subscript access to the Matrix4x4, const

	  //Print overloading
	     friend  ostream&  operator<<(ostream& output, const Matrix4x4& m);
	  //Print the Matrix4x4 to a text file for easy debug
		 void debugPrint();


	  //Access to rows and columns
	     Vector Matrix4x4::getRow(unsigned i);
	     Vector Matrix4x4::getCol(unsigned i);
	     void Matrix4x4::setRow(unsigned i,const Vector& v);
	     void Matrix4x4::setCol(unsigned i,const Vector& v);
		 void Matrix4x4::swapRows(unsigned i, unsigned j);
		 Matrix4x4 Matrix4x4::getRotation();

	  //Transposition
	     friend Matrix4x4  operator~(const Matrix4x4 &m);

	  //Inverse Matrix4x4
	     friend Matrix4x4  operator!(const Matrix4x4 &m);
	  
	  //BEWARE! THIS METHODS COMMUTE THE CURRENT Matrix4x4 IN THE SPECIFIED ONE!
	  //THE PREVIOUS Matrix4x4 WILL BE LOST!
	     void Matrix4x4::uScale(const float& f);                //uniform Scale Matrix4x4, see http://en.wikipedia.org/wiki/Scaling_%28geometry%29
	     void Matrix4x4::translate(const Vector &v);             //Translation Matrix4x4
	     void Matrix4x4::shear(const float& n,const unsigned& i,const unsigned& j);  //Create a Shear Matrix4x4, see http://en.wikipedia.org/wiki/Shear_Matrix4x4
	     void Matrix4x4::rotX(const double& a);                        // x axis 
         void Matrix4x4::rotY(const double& a);                        // y axis 
         void Matrix4x4::rotZ(const double& a);                        // z axis 
	     void Matrix4x4::rotV(const double& a,const Vector& v);        // rotation with respect to an axes
      
	  
	  //Multiplication operators
	     Matrix4x4& operator *= (const float& f);
	     Matrix4x4& operator *= (const Matrix4x4 &m);
	  
         friend Matrix4x4  operator * (const Matrix4x4 &m,const float& f);	
	     friend Matrix4x4  operator * (const float& f,const Matrix4x4 &m);
	     friend Matrix4x4  operator * (const Matrix4x4 &m1,const Matrix4x4 &m2);

	 //Equality tests
		 friend bool operator == (const Matrix4x4 &a, const Matrix4x4 &b);

		Quaternion Matrix4x4::createQuaternion();
};
#endif;