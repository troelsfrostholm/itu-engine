#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	for(unsigned i=0;i<16;i++)
	{
		if((0==i)||(5==i)||(10==i)||(15==i))
			elements[i]=1;
		else
			elements[i]=0;
	}
}

 Matrix4x4 Matrix4x4::getRotation()
 {
	 Matrix4x4 toBeReturned= Matrix4x4();
	 toBeReturned=(*this);
	 toBeReturned.setCol(4,Vector(0,0,0));
	 toBeReturned.setRow(4,Vector(0,0,0));
	 return toBeReturned;
 }

float& Matrix4x4::operator() (unsigned row, unsigned col)
 {
   if (row > 4 || col > 4)
     throw "Matrix4x4 error: Index out of range";
   return elements[4*(row-1) + (col-1)];
 }
 
 const float Matrix4x4::operator() (unsigned row, unsigned col) const
 {
   if (row > 4 || col > 4)
     throw "Matrix4x4 error: Index out of range";
   return elements[4*(row-1) + (col-1)];
 } 
 
ostream& operator<<(ostream& output, const Matrix4x4& m) {
	unsigned i;
	unsigned j;
	for(i=1;i<=4;i++)
	{
		for(j=1;j<=4;j++)
		{
			output<<m(i,j)<<' ';
		}
		output<<'\n';
	}
    return output;
}

Vector Matrix4x4::getRow(unsigned i)
{
	if(1 > i || i > 4)
	{
		throw "Matrix4x4 error: Index out of range. ";
	}
	else
	{
		return Vector((*this)(i,1),(*this)(i,2),(*this)(i,3),(*this)(i,4));
	}
}

Vector Matrix4x4::getCol(unsigned i)
{
	if(1 > i || i > 4)
	{
		throw "Matrix4x4 error: Index out of range. ";
	}
	else
	{
		return Vector((*this)(1,i),(*this)(2,i),(*this)(3,i),(*this)(4,i));
	}
}

void Matrix4x4::setRow(unsigned i,const Vector& v)
{
	if(1 > i || i > 4)
	{
		throw "Matrix4x4 error: Index out of range. ";
	}
	else
	{
		for(unsigned j=1;j<=4;j++)
		{
			(*this)(i,j)=v[j];
		}
	}
}
void Matrix4x4::setCol(unsigned i,const Vector& v)
{
	if(1 > i || i > 4)
	{
		throw "Matrix4x4 error: Index out of range. ";
	}
	else
	{
		for(unsigned j=1;j<=4;j++)
		{
			(*this)(j,i)=v[j];
		}
	}
}

 Matrix4x4 operator~(const Matrix4x4 &m)
 {
	 Matrix4x4 temp;
	 for(unsigned i=1;i<=4;i++)
 		 for(unsigned j=1;j<=4;j++)
		 {
			 (temp)(j,i)=m(i,j);;
		 }
	 return temp;
 }

 void Matrix4x4::swapRows(unsigned i, unsigned j)
 {
	 float temp;
	 for(unsigned k=1; k <= 4; k++)
	 {
		 temp = (*this)(i,k);
		 (*this)(i,k) = (*this)(j,k);
		 (*this)(j,k) = temp;
	 }
 }

 Matrix4x4 operator!(const Matrix4x4 &m)
 {
	Matrix4x4 U = Matrix4x4(); //inverse to be found
	Matrix4x4 tmp = m;
	int i = 1;
	int j = 1;
	int M = 4+1;
	int N = 4+1;
		
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
	return U;
 }

 void Matrix4x4::rotX(const double& angle)
 {
	 Matrix4x4 rot=Matrix4x4();
	 rot(2,2)=nmsTrig::cos(angle);
	 rot(2,3)=-1*nmsTrig::sin(angle);
	 rot(3,2)=nmsTrig::sin(angle);
	 rot(3,3)=nmsTrig::cos(angle);
	 (*this)=rot;
 }

 void Matrix4x4::rotY(const double& angle)  
 {
	 Matrix4x4 rot=Matrix4x4();
	 rot(1,1)=nmsTrig::cos(angle);
	 rot(1,3)=nmsTrig::sin(angle);
	 rot(3,1)=-1*nmsTrig::sin(angle);
	 rot(3,3)=nmsTrig::cos(angle);
	 (*this)=rot;
 }

 void Matrix4x4::rotZ(const double& angle)
 {
	 Matrix4x4 rot=Matrix4x4();
	 rot(1,1)=nmsTrig::cos(angle);
	 rot(1,2)=-1*nmsTrig::sin(angle);
	 rot(2,1)=nmsTrig::sin(angle);
	 rot(2,2)=nmsTrig::cos(angle);
	 (*this)=rot;
 }

 void Matrix4x4::rotV(const double& a,const Vector& v)
 {
	 Matrix4x4 rot=Matrix4x4();
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

bool operator == (const Matrix4x4 &a, const Matrix4x4 &b)
{
	return ( memcmp(a.getElements(), b.getElements(), 16*sizeof(float)) == 0);
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& m)
{
   Matrix4x4 temp;

   for (unsigned i=1; i <= 4; i++)
      for (unsigned j=1; j <= 4; j++)
      {
		 temp(i,j) = 0;
         for (unsigned k=1; k <= 4; k++)
            temp(i,j) += (*this)(i,k) * m(k,j);
      }
   *this = temp;

   return *this;
}


Matrix4x4& Matrix4x4::operator *= (const float& f) 
{
	for (unsigned i=1; i < 4; i++)
		for (size_t j=1; j < 4; j++)
			(*this)(i,j) *= f;
    return *this;
}

Matrix4x4 operator * (const Matrix4x4& m, const float& f)
{
   Matrix4x4 temp = m;
   temp *= f;
   return temp;
}

Matrix4x4 operator * (const float& f,const Matrix4x4& m)
{
   return (m * f);
}

Matrix4x4 operator * (const Matrix4x4& m1, const Matrix4x4& m2)
{
   Matrix4x4 temp = m1;
   temp *= m2;
   return temp;
}

void Matrix4x4::uScale(const float& f)
{
	(*this)=Matrix4x4();
	for(unsigned i=1;i<4;i++)
		(*this)(i,i)=f;
}

void Matrix4x4::translate(const Vector &v)
{
	(*this)=Matrix4x4();
	(*this)(1,4)=v[1];
	(*this)(2,4)=v[2];
	(*this)(3,4)=v[3];
}

void Matrix4x4::shear(const float& n,const unsigned& i,const unsigned& j)
{
	if (i==j)
    {
	   throw "Matrix4x4 error: Cannot give the same index to obtain a shear Matrix4x4!\n";
    }
	(*this)=Matrix4x4();
	(*this)(i,j)=n;
}

const float* Matrix4x4::getElements() const
{
	return elements;
}

void Matrix4x4::debugPrint()
{
	 char buffer [50];
	 size_t buffer_size = 50*sizeof(char);
	 FILE	*fp;
	 fopen_s(&fp, "Matrix4x4Debug.txt","a");
	 if (!fp)
		return;
	 for(unsigned i=1;i<=4; i++)
	 {
		for(unsigned j=1;j<=4;j++)
		{
			sprintf_s(buffer, buffer_size, "%f ", (*this)(i,j));fwrite(buffer,strlen(buffer),1,fp);
		}
		sprintf_s(buffer, buffer_size, "\n");fwrite(buffer,strlen(buffer),1,fp);
	 }
	 sprintf_s(buffer, buffer_size, "\n");fwrite(buffer,strlen(buffer),1,fp);
	 fclose(fp);

}

Quaternion  Matrix4x4::createQuaternion()
{
	Quaternion toBeReturned;
	toBeReturned[NMS_W] = sqrt( max( 0.f, 1.f + (*this)(1,1) + (*this)(2,2) +  (*this)(3,3) ) ) / 2;
	toBeReturned[NMS_X] = sqrt( max( 0.f, 1.f + (*this)(1,1) - (*this)(2,2)  - (*this)(3,3) ) ) / 2;
	toBeReturned[NMS_Y] = sqrt( max( 0.f, 1.f - (*this)(1,1) + (*this)(2,2)  - (*this)(3,3) ) ) / 2;
	toBeReturned[NMS_Z] = sqrt( max( 0.f, 1.f - (*this)(1,1) - (*this)(2,2)  + (*this)(3,3) ) ) / 2;
	toBeReturned[NMS_X] =(float) _copysign( toBeReturned[NMS_X], (*this)(3,2) - (*this)(2,3) );
	toBeReturned[NMS_Y] =(float) _copysign( toBeReturned[NMS_Y], (*this)(1,3) - (*this)(3,1) );
	toBeReturned[NMS_Z] =(float) _copysign( toBeReturned[NMS_Z], (*this)(2,1) - (*this)(1,2) );
	return toBeReturned;
}