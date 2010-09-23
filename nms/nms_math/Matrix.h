#ifndef MATRIX_H__
#define MATRIX_H__

#include <fstream>
#include "Vector.h"
#include "Trig.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

class Matrix{
  
   private:

	 struct mat_struct
		{
		float *elements;
		unsigned o_rows, o_cols;
		unsigned refCounter;

		mat_struct (unsigned row, unsigned col, float* val)
		{
			o_rows=row;
			o_cols=col;
			refCounter = 1;

			elements = new float [o_rows*o_cols];
		}
		~mat_struct ()
		{
			delete [] elements;
			elements = NULL;
		}
	 };

	 mat_struct* structPointer;

	 //Used to allocate and resize a matrix
	 void Matrix::memoryRealloc (unsigned row, unsigned col,unsigned startRow,unsigned startCol);
	 //Used to pivoting on the column specified
	 Matrix Matrix::pivot(unsigned column);

  public:
	  //CONSTRUCTORS
	     Matrix::Matrix();		   //Create a default 4x4 Identity matrix
	     Matrix::Matrix(unsigned rows,unsigned cols);
	     Matrix::Matrix(unsigned rows,unsigned cols,float* toBeCopied);
	  //copy constructor
	     Matrix::Matrix(const Matrix& m);
	     Matrix::~Matrix();

		 float* Matrix::returnPointer();

	  //Access to the matrix values, starting from 1 NOT 0
	     float& Matrix::operator()(unsigned row,unsigned col);		//Subscript access to the matrix, non const
	     float const Matrix::operator()(unsigned row,unsigned col) const;	//Subscript access to the matrix, const

	  //Print overloading
	     friend  ostream&  operator<<(ostream& output,Matrix& m);
	  //Print the matrix to a text file for easy debug
		 void debugPrint();


	  //Access to rows and columns
	     Vector Matrix::getRow(unsigned i);
	     Vector Matrix::getCol(unsigned i);
	     void Matrix::setRow(unsigned i,const Vector& v);
	     void Matrix::setCol(unsigned i,const Vector& v);
	     unsigned Matrix::getRowL() const;
	     unsigned Matrix::getColL() const;
 


	  //Transposition
	     friend Matrix  operator~(const Matrix &m);

	  //Inverse matrix
	     friend Matrix  operator!(const Matrix &m);
	  
	  //BEWARE! THIS METHODS COMMUTE THE CURRENT MATRIX IN THE SPECIFIED ONE!
	  //THE PREVIOUS MATRIX WILL BE LOST!
	     void Matrix::uScale(const float& f);                //uniform Scale matrix, see http://en.wikipedia.org/wiki/Scaling_%28geometry%29
	     void Matrix::translate(const Vector &v);             //Translation Matrix
	     void Matrix::shear(const float& n,const unsigned& i,const unsigned& j);  //Create a Shear Matrix, see http://en.wikipedia.org/wiki/Shear_matrix
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
	
	     void Matrix::resize (unsigned row, unsigned col,unsigned startRow,unsigned startCol);
	  

	  //Copy operator
	     const Matrix& operator = (const Matrix& m)
		{
			m.structPointer->refCounter++;
			if(--structPointer->refCounter == 0) 
			{ delete structPointer;
			  structPointer=NULL;
			}
			structPointer=m.structPointer;
			return *this;
		}
};
#endif;