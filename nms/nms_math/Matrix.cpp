#include "Matrix.h"

Matrix::Matrix()
{
	structPointer = new mat_struct(4,4);
	for(unsigned i=0;i<16;i++)
	{
		if((0==i)||(5==i)||(10==i)||(15==i))
			structPointer->elements[i]=1;
		else
			structPointer->elements[i]=0;
	}
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
   structPointer = new mat_struct(rows,cols);
   for(unsigned i=0;i<rows*cols;i++)
			structPointer->elements[i]=0;
 }

Matrix::Matrix(const Matrix& m)
{
	structPointer = new mat_struct(m.getRowL(),m.getColL());
	for(unsigned i=0; i<m.getRowL();i++){
		for(unsigned j=0; j<m.getColL();j++){
			structPointer->elements[i*m.getColL()+j] = m(i+1,j+1);
		}
	}

}

const Matrix& Matrix::operator = (const Matrix& m)
{
	if (this != &m) {
		delete structPointer;
		structPointer = new mat_struct(m.getRowL(),m.getColL());
		for(unsigned i=0; i<m.getRowL();i++){
			for(unsigned j=0; j<m.getColL();j++){
				structPointer->elements[i*m.getColL()+j] = m(i+1,j+1);
			}
		}
	}
	return *this;
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
 
ostream& operator<<(ostream& output, const Matrix& m) {
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


 void Matrix::swapRows(unsigned i, unsigned j)
 {
	 float temp;
	 for(unsigned k=1; k <= (*this).getColL(); k++)
	 {
		 temp = (*this)(i,k);
		 (*this)(i,k) = (*this)(j,k);
		 (*this)(j,k) = temp;
	 }
 }

 Matrix operator!(const Matrix &m)
 {
	Matrix U = Matrix(); //inverse to be found
	Matrix tmp = m;
	int i = 1;
	int j = 1;
	int M = tmp.getRowL()+1;
	int N = tmp.getColL()+1;
	if(M != N)
	{
		 //To be Fixed
		 throw 0;
	}
	else
	{
		while (i < M && j < N)
		{
			//find the biggest element
			int maxi = i;
			for(int k = i+1; k<M; k++)
			{
				if(abs(tmp(k,j)) > abs(tmp(maxi,j)))
				{
					maxi = k;
				}
			}
			
			//use gaussian elemination
			if(tmp(maxi,j) != 0)
			{
				//if row not in the right place - swap
				if(i != maxi){
					tmp.swapRows(i, maxi);
					U.swapRows(i,maxi);
				}

				float divisor = tmp(i,j);
				for(int k=1; k<N; k++)
				{
					U(i,k) = U(i,k)/divisor;
					tmp(i,k) = tmp(i,k)/divisor;
				}

				for(int k=1; k<M; k++)
				{
					if(k != i)
					{
						float multiplier = tmp(k,j);
						for(int l=1; l<N; l++)
						{
							U(k,l) = U(k,l) - U(i,l)*multiplier;
							tmp(k,l) = tmp(k,l) - tmp(i,l)*multiplier;
						}
					}
				}
				i++;
			}
			j++;
		}
	}
	return U;
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

bool operator == (const Matrix &a, const Matrix &b)
{
	if( a.getColL() != b.getColL() || 
		a.getRowL() != b.getRowL() )
		return false;

	for(unsigned i=1; i<=a.getRowL(); i++) {
		for(unsigned j=1; j<=a.getColL(); j++) {
			if( a(i, j)!=b(i, j) ) return false;
		}
	}
	return true;
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
	 size_t buffer_size = 50*sizeof(char);
	 FILE	*fp;
	 fopen_s(&fp, "MatrixDebug.txt","a");
	 if (!fp)
		return;
	 for(unsigned i=1;i<=(*this).getRowL();i++)
	 {
		for(unsigned j=1;j<=(*this).getColL();j++)
		{
			sprintf_s(buffer, buffer_size, "%f ", (*this)(i,j));fwrite(buffer,strlen(buffer),1,fp);
		}
		sprintf_s(buffer, buffer_size, "\n");fwrite(buffer,strlen(buffer),1,fp);
	 }
	 sprintf_s(buffer, buffer_size, "\n");fwrite(buffer,strlen(buffer),1,fp);
	 fclose(fp);

}