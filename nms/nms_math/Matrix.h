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
		}
	 };

	 mat_struct* structPointer;

	 //Used to allocate and resize a matrix
	 void Matrix::memoryRealloc (unsigned row, unsigned col,unsigned startRow,unsigned startCol);
	 //Used to pivoting on the column specified
	 Matrix Matrix::pivot(unsigned column);

  public:
	  //CONSTRUCTORS
	    __declspec(dllexport) Matrix::Matrix();		   //Create a default 4x4 Identity matrix
	    __declspec(dllexport) Matrix::Matrix(unsigned rows,unsigned cols);
	    __declspec(dllexport) Matrix::Matrix(unsigned rows,unsigned cols,float* toBeCopied);
	  //copy constructor
	    __declspec(dllexport) Matrix::Matrix(const Matrix& m);
	    __declspec(dllexport) Matrix::~Matrix();

	  //Access to the matrix values, starting from 1 NOT 0
	    __declspec(dllexport) float& Matrix::operator()(unsigned row,unsigned col);		//Subscript access to the matrix, non const
	    __declspec(dllexport) float const Matrix::operator()(unsigned row,unsigned col) const;	//Subscript access to the matrix, const

	  //Print overloading
	    __declspec(dllexport) friend ostream& operator<<(ostream& output,Matrix& m);


	  //Access to rows and columns
	    __declspec(dllexport) Vector Matrix::getRow(unsigned i);
	    __declspec(dllexport) Vector Matrix::getCol(unsigned i);
	    __declspec(dllexport) void Matrix::setRow(unsigned i,const Vector& v);
	    __declspec(dllexport) void Matrix::setCol(unsigned i,const Vector& v);
	    __declspec(dllexport) unsigned Matrix::getRowL() const;
	    __declspec(dllexport) unsigned Matrix::getColL() const;
 


	  //Transposition
	    __declspec(dllexport) friend Matrix  operator~(const Matrix &m);

	  //Inverse matrix
	    __declspec(dllexport) friend Matrix operator!(const Matrix &m);
	  
	  //BEWARE! THIS METHODS COMMUTE THE CURRENT MATRIX IN THE SPECIFIED ONE!
	  //THE PREVIOUS MATRIX WILL BE LOST!
	    __declspec(dllexport) void Matrix::uScale(const float& f);                //uniform Scale matrix, see http://en.wikipedia.org/wiki/Scaling_%28geometry%29
	    __declspec(dllexport) void Matrix::translate(const Vector &v);             //Translation Matrix
	    __declspec(dllexport) void Matrix::shear(const float& n,const unsigned& i,const unsigned& j);  //Create a Shear Matrix, see http://en.wikipedia.org/wiki/Shear_matrix
	    __declspec(dllexport) void Matrix::rotX(const double& a);                        // x axis 
        __declspec(dllexport) void Matrix::rotY(const double& a);                        // y axis 
        __declspec(dllexport) void Matrix::rotZ(const double& a);                        // z axis 
	    __declspec(dllexport) void Matrix::rotV(const double& a,const Vector& v);        // rotation with respect to an axes
      
	  
	  //Multiplication operators
	    __declspec(dllexport) Matrix& operator *= (const float& f);
	    __declspec(dllexport) Matrix& operator *= (const Matrix &m);
	  
        __declspec(dllexport) friend Matrix  operator * (const Matrix &m,const float& f);	
	    __declspec(dllexport) friend Matrix  operator * (const float& f,const Matrix &m);
	    __declspec(dllexport) friend Matrix  operator * (const Matrix &m1,const Matrix &m2);
	  
	  
	
	    __declspec(dllexport) void Matrix::resize (unsigned row, unsigned col,unsigned startRow,unsigned startCol);
	  

	  //Copy operator
	    __declspec(dllexport) const Matrix& operator = (const Matrix& m)
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