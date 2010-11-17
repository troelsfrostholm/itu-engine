#define Matrix_D __declspec(dllexport)


#ifndef Matrix_H__
#define Matrix_H__

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

class Matrix_D Matrix{
	friend class Quaternion;
private:
	   float elements[16];

  public:
	  //CONSTRUCTORS & destructor
	     Matrix::Matrix();		           //Creates identity matrix
	     Matrix::Matrix(float* _elements);

		 const float* Matrix::getElements() const;             //return matrix elements as float array

	  //Access to the Matrix values, starting from 1 NOT 0
	     float& Matrix::operator()(unsigned row,unsigned col);		//Subscript access to the Matrix, non const
	     float const Matrix::operator()(unsigned row,unsigned col) const;	//Subscript access to the Matrix, const

	  //Print overloading
	     friend  ostream&  operator<<(ostream& output, const Matrix& m);
	  //Print the Matrix to a text file for easy debug
		 void debugPrint();


	  //Access to rows and columns
	     Vector Matrix::getRow(unsigned i);
	     Vector Matrix::getCol(unsigned i);
	     void Matrix::setRow(unsigned i,const Vector& v);
	     void Matrix::setCol(unsigned i,const Vector& v);
		 void Matrix::swapRows(unsigned i, unsigned j);
		 Matrix Matrix::getRotation();

	  //Transposition
	     friend Matrix  operator~(const Matrix &m);

	  //Inverse Matrix
	     friend Matrix  operator!(const Matrix &m);
	  
	  //BEWARE! THIS METHODS COMMUTE THE CURRENT Matrix IN THE SPECIFIED ONE!
	  //THE PREVIOUS Matrix WILL BE LOST!
	     void Matrix::uScale(const float& f);                //uniform Scale Matrix, see http://en.wikipedia.org/wiki/Scaling_%28geometry%29
	     void Matrix::translate(const Vector &v);             //Translation Matrix
	     void Matrix::shear(const float& n,const unsigned& i,const unsigned& j);  //Create a Shear Matrix, see http://en.wikipedia.org/wiki/Shear_Matrix
	     void Matrix::rotX(const double& a);                        // x axis 
         void Matrix::rotY(const double& a);                        // y axis 
         void Matrix::rotZ(const double& a);                        // z axis 
	     void Matrix::rotV(const double& a,const Vector& v);        // rotation with respect to an axes
      
	  
	  //Multiplication operators
	     Matrix& operator *= (const float& f);
	     Matrix& operator *= (const Matrix &m);
	  
         friend Matrix  operator * (const Matrix &m,const float& f);	
	     friend Matrix  operator * (const float& f,const Matrix &m);
	     friend Matrix  operator * (const Matrix &m1,const Matrix &m2);

	 //Equality tests
		 friend bool operator == (const Matrix &a, const Matrix &b);
		 
		 Quaternion createQuaternion();
};
#endif;