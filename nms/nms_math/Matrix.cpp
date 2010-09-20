#include "Matrix.h"



Matrix::Matrix()
{
	structPointer = new mat_struct(4,4,0);
	for(unsigned i=0;i<16;i++)
	{
		if((0==i)||(5==i)||(10==i)||(15==i))
			structPointer->elements[i]=1;
		else
			structPointer->elements[i]=0;
	}
}

Matrix::Matrix(unsigned rows,unsigned cols,float* toBeCopied)
{
	structPointer = new mat_struct(rows,cols,toBeCopied);
}


Matrix::~Matrix()
 {
   ++structPointer->refCounter;
   if (--structPointer->refCounter == 0)
   {
	   delete structPointer;
	   structPointer=NULL;
   }
 }

Matrix::Matrix(unsigned rows, unsigned cols)
 {
   if (rows == 0 || cols == 0)
     throw 0;
   structPointer = new mat_struct(rows,cols,0);
   for(unsigned i=0;i<rows*cols;i++)
			structPointer->elements[i]=0;
 }

Matrix::Matrix(const Matrix& m)
{
	structPointer=m.structPointer;
	structPointer->refCounter++;
}

float& Matrix::operator() (unsigned row, unsigned col)
 {
   if (row > structPointer->o_rows || col > structPointer->o_cols)
     throw 0;
   return structPointer->elements[structPointer->o_cols*(row-1) + (col-1)];
 }
 
 const float Matrix::operator() (unsigned row, unsigned col) const
 {
	 if (row > structPointer->o_rows || col > structPointer->o_cols)
       throw 0;
     return structPointer->elements[structPointer->o_cols*(row-1) + (col-1)];
 } 
 
 unsigned Matrix::getRowL() const
 {
	 return structPointer->o_rows;
 }

 unsigned Matrix::getColL() const
 {
	return this->structPointer->o_cols;
 }
 
ostream& operator<<(ostream& output,Matrix& m) {
	unsigned i;
	unsigned j;
	for(i=1;i<=m.getRowL();i++)
	{
		for(j=1;j<=m.getColL();j++)
		{
			output<<m(i,j)<<' ';
		}
		output<<'\n';
	}
    return output;
}

Vector Matrix::getRow(unsigned i)
{
	if((*this).getRowL()!=4&&(*this).getColL()!=4)
	{
		cout<<"Implementation valid only for 4x4 matrix!"<<'\n';
		throw 0;
	}
	else
	{
		return Vector((*this)(i,1),(*this)(i,2),(*this)(i,3),(*this)(i,4));
	}
}

Vector Matrix::getCol(unsigned i)
{
	if((*this).getRowL()!=4&&(*this).getColL()!=4)
	{
		cout<<"Implementation valid only for 4x4 matrix!"<<'\n';
		throw 0;
	}
	else
	{
		return Vector((*this)(1,i),(*this)(2,i),(*this)(3,i),(*this)(4,i));
	}
}

void Matrix::setRow(unsigned i,const Vector& v)
{
	if((*this).getRowL()!=4&&(*this).getColL()!=4)
	{
		cout<<"Implementation valid only for 4x4 matrix!"<<'\n';
		throw 0;
	}
	else
	{
		for(unsigned j=1;j<=4;j++)
		{
			(*this)(i,j)=v[j];
		}
	}
}
void Matrix::setCol(unsigned i,const Vector& v)
{
	if((*this).getRowL()!=4&&(*this).getColL()!=4)
	{
		cout<<"Implementation valid only for 4x4 matrix!"<<'\n';
		throw 0;
	}
	else
	{
		for(unsigned j=1;j<=4;j++)
		{
			(*this)(j,i)=v[j];
		}
	}
}

 Matrix operator~(const Matrix &m)
  {
	 Matrix temp(m.getColL(),m.getRowL());
	 for(unsigned i=1;i<=temp.getRowL();i++)
 		 for(unsigned j=1;j<=temp.getColL();j++)
		 {
			 (temp)(j,i)=m(i,j);;
		 }
	 return temp;
  }


 Matrix Matrix::pivot(unsigned column)
 {
	 Matrix temp=(*this);
	 float maxValue=abs(temp(1,1));
     unsigned maxPos=1;
	 //find the line with the biggest value
	 for(unsigned i=column;i<=temp.getRowL();i++)
		 {
			if(abs((*this)(i,column))>maxValue)
			{
				maxValue=temp(i,column);
				maxPos=i;
			}
		 }
	  if(column!=maxPos)
	  {
		  //Substitute the columns
		  for(unsigned j=column;j<=temp.getColL();j++)
		  {
			  maxValue=temp(column,j);
			  temp(column,j)=temp(maxPos,j);
			  temp(maxPos,j)=maxValue;
		  }
	  }
	  return temp;
 }

 Matrix operator!(const Matrix &m)
  {
	 if(m.getRowL() != m.getColL())
	 {
		 //To be Fixed
		 throw 0;
	 }
	 else
	 {
		 //Calculate the U matrix
		 bool unswapped=true;
		 Matrix U=Matrix(m.getRowL(),m.getColL()+m.getRowL());
		 for(unsigned i=1;i<=U.getRowL();i++)
		 {
			 for(unsigned j=1;j<=U.getColL();j++)
			 //Create the [A|I] matrix in the copy
			 {
				if(j<=m.getColL())
					U(i,j)=m(i,j);
				else if(j==(m.getColL()+i))
					U(i,j)=1;
				else
					U(i,j)=0;
			 }
		 }
		 //Now apply gauss elimination
		 for(unsigned i=1;i<=U.getRowL();i++)
		 {
			 //Do not swap rows if it is the last row!
			 if(i!=U.getRowL())
				U=U.pivot(i);
			 float divisor=U(i,i);
			 //Normalize the row
			 for(unsigned j=i;j<=U.getColL();j++)
			 {
				 U(i,j)=U(i,j)/divisor;
			 }
			 //Now subtract the row from the following ones
			 for(unsigned z=1;z<=U.getRowL();z++)
			 {
				 if(z!=i)
				 {
					 float multiplier=U(z,i);
					 for(unsigned j=i;j<=U.getColL();j++)
					 {
						 U(z,j)=U(z,j)-multiplier*U(i,j);
					 }
				 }
			 }
		 }
		 //Resize to get only the right part of the matrix, the inverse
		 U.resize(m.getRowL(),m.getColL(),1,m.getColL()+1);
		 return U;
	 }
  }


 void Matrix::rotX(const double& angle)
 {
	 Matrix rot=Matrix();
	 rot(2,2)=nmsTrig::cos(angle);
	 rot(2,3)=-1*nmsTrig::sin(angle);
	 rot(3,2)=nmsTrig::sin(angle);
	 rot(3,3)=nmsTrig::cos(angle);
	 (*this)=rot;
 }

 void Matrix::rotY(const double& angle)  
 {
	 Matrix rot=Matrix();
	 rot(1,1)=nmsTrig::cos(angle);
	 rot(1,3)=nmsTrig::sin(angle);
	 rot(3,1)=-1*nmsTrig::sin(angle);
	 rot(3,3)=nmsTrig::cos(angle);
	 (*this)=rot;
 }

 void Matrix::rotZ(const double& angle)
 {
	 Matrix rot=Matrix();
	 rot(1,1)=nmsTrig::cos(angle);
	 rot(1,2)=-1*nmsTrig::sin(angle);
	 rot(2,1)=nmsTrig::sin(angle);
	 rot(2,2)=nmsTrig::cos(angle);
	 (*this)=rot;
 }

 void Matrix::rotV(const double& a,const Vector& v)
 {
	 Matrix rot=Matrix();
	 rot(1,1)=v[NMS_X]*v[NMS_X]+(1-(v[NMS_X]*v[NMS_X]))*nmsTrig::cos(a);
	 rot(2,1)=v[NMS_X]*v[NMS_Y]*(1-nmsTrig::cos(a))+v[NMS_Z]*nmsTrig::sin(a);
	 rot(3,1)=v[NMS_X]*v[NMS_Z]*(1-nmsTrig::cos(a))-v[NMS_Y]*nmsTrig::sin(a);
	 rot(1,2)=v[NMS_X]*v[NMS_Y]*(1-nmsTrig::cos(a))-v[NMS_Z]*nmsTrig::sin(a);
	 rot(2,2)=v[NMS_Y]*v[NMS_Y]+(1-v[NMS_Y]*v[NMS_Y])*nmsTrig::cos(a);
	 rot(3,2)=v[NMS_Y]*v[NMS_Z]*(1-nmsTrig::cos(a))+v[NMS_X]*nmsTrig::sin(a);
	 rot(1,3)=v[NMS_X]*v[NMS_Z]*(1-nmsTrig::cos(a))+v[NMS_Y]*nmsTrig::sin(a);
	 rot(2,3)=v[NMS_Y]*v[NMS_Z]*(1-nmsTrig::cos(a))-v[NMS_X]*nmsTrig::sin(a);
	 rot(3,3)=v[NMS_Z]*v[NMS_Z]+(1-v[NMS_Z]*v[NMS_Z])*nmsTrig::cos(a);
	 (*this)=rot;
 }

//IMPORTANT! TRY TO USE MEMCPY FOR FASTER IMPLEMENTATION!
//Reallocation of memory for matrix: it takes the number of rows and columns for the final matrix
//and the starting position from which begin the copy
void Matrix::memoryRealloc (unsigned row, unsigned col,unsigned startRow,unsigned startCol)
{
	if (structPointer->o_rows == row && structPointer->o_cols == col && 1==startRow && 1==startCol)
    {
      return;
    }

   Matrix temp=Matrix(row,col);
   unsigned minCol=min(structPointer->o_cols,col);
   unsigned maxCol=max(structPointer->o_cols,col);
   size_t colSpace=maxCol*sizeof(float);
   unsigned minRow=min(structPointer->o_rows,row);
   

   //fill blank spaces
   for(unsigned i=0;i<minRow;i++)
	   for(unsigned j=0;j<minCol;j++)
		   temp.structPointer->elements[i]=0;

    for(unsigned i=1;i<=minRow;i++)
	   for(unsigned j=1;j<=minCol;j++)
		   temp(i,j)=(*this)(i+(startRow-1),j+(startCol-1));

   (*this)=temp;

   return;
}

//Public method to resize matrixes
void Matrix::resize (unsigned row, unsigned col,unsigned startRow,unsigned startCol)
{
   //counters
   unsigned i,j;

   unsigned tempRow = (*this).getRowL();
   unsigned tempCol = (*this).getColL();

   if (row != tempRow || col != tempCol)
      memoryRealloc( row, col,startRow,startCol);

   for (i=tempRow; i < row; i++)
   {
      for (j=0; j < col; j++)
	  {
	    (*this)(i,j) = 0;
	  }
   }

   for (i=0; i < row; i++)  
   {
      for (j=tempCol; j < col; j++)
	  {
		(*this)(i,j) = 0;
	  }
   }
   return;
}

Matrix& Matrix::operator *= (const Matrix& m)
{
   if ((*this).getColL() != m.getRowL())
   {
	   cerr<<"The dimensions of the matrix do not match for the * operator!\n";
	   //TO BE FIXED
      throw 0;
   }

   Matrix temp((*this).getRowL(),m.getColL());

   for (unsigned i=1; i <= (*this).getRowL(); i++)
      for (unsigned j=1; j <= m.getColL(); j++)
      {
		 temp(i,j) = 0;
         for (unsigned k=1; k <= (*this).getColL(); k++)
            temp(i,j) += (*this)(i,k) * m(k,j);
      }
   *this = temp;

   return *this;
}


Matrix& Matrix::operator *= (const float& f) 
{
   // if (_m->Refcnt > 1) clone();
	for (unsigned i=1; i < (*this).getRowL(); i++)
		for (size_t j=1; j < (*this).getColL(); j++)
			(*this)(i,j) *= f;
    return *this;
}

Matrix operator * (const Matrix& m, const float& f)
{
   Matrix temp = m;
   temp *= f;
   return temp;
}

Matrix operator * (const float& f,const Matrix& m)
{
   return (m * f);
}

Matrix operator * (const Matrix& m1, const Matrix& m2)
{
   Matrix temp = m1;
   temp *= m2;
   return temp;
}

void Matrix::uScale(const float& f)
{
	(*this)=Matrix();
	for(unsigned i=1;i<(*this).getRowL();i++)
		(*this)(i,i)=f;
}

void Matrix::translate(const Vector &v)
{
	(*this)=Matrix();
	(*this)(1,4)=v[1];
	(*this)(2,4)=v[2];
	(*this)(3,4)=v[3];
}

void Matrix::shear(const float& n,const unsigned& i,const unsigned& j)
{
	if (i==j)
    {
	   cerr<<"Cannot give the same index to obtain a shear matrix!\n";
	   //TO BE FIXED
       throw 0;
    }
	(*this)=Matrix();
	(*this)(i,j)=n;
}

float* Matrix::returnPointer()
{
	return (*this).structPointer->elements;
}

void Matrix::debugPrint()
{
	 char buffer [50];
	 FILE	*fp;
	 fp=fopen("MatrixDebug.txt","a");
	 if (!fp)
		return;
	 for(unsigned i=1;i<=(*this).getRowL();i++)
	 {
		for(unsigned j=1;j<=(*this).getColL();j++)
		{
			sprintf(buffer,"%f ", (*this)(i,j));fwrite(buffer,strlen(buffer),1,fp);
		}
		sprintf(buffer,"\n");fwrite(buffer,strlen(buffer),1,fp);
	 }
	 sprintf(buffer,"\n");fwrite(buffer,strlen(buffer),1,fp);
	 fclose(fp);

}