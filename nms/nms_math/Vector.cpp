#include "Vector.h"
#include "Matrix.h"


using namespace std;

Vector::Vector()
{
	Point();
	w=0;
}

Vector::Vector(float x,float y,float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
	w=0;
}

Vector::Vector(float x,float y,float z,float w)
{
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
}


Vector& Vector::operator+=(Vector const &v)
{
	x+=v[NMS_X]; y+=v[NMS_Y]; z+=v[NMS_Z];
	return (*this);
}

Vector& Vector::operator-=(Vector const &v)
{
	x-=v[NMS_X]; y-=v[NMS_Y]; z-=v[NMS_Z];
	return (*this);
}

Vector&  Vector::operator*=(float f)
{
	x*=f; y*=f; z*=f;
	return (*this);
}

Vector&  Vector::operator/=(float f)
{
	x/=f; y/=f; z/=f;
	return (*this);
}

Vector Vector::operator*(float f)
{
	return Vector(x*f,y*f,z*f);
}

Vector& Vector::operator *= (const Matrix &m)
{
	if (4!=m.getRowL())
	{
		cerr<<"The dimension of the row of the matrix do not match for the * operator, it should be 4!\n";
	   //TO BE FIXED
      throw 0;
	}

	Vector temp=Vector();
	for (unsigned i=1; i <= 4; i++)
	{
	  temp[i] = 0;
      for (unsigned j=1; j <= 4; j++)
      {
         temp[i] += (*this)[j] * m(j,i);
      }
	}
	(*this)=temp;
	return (*this);
}

Vector operator * (Vector& v,const Matrix &m)
{
	Vector temp = v;
    temp *= m;
    return temp;
}


Vector  Vector::operator+(Vector const &v)
{
	return Vector(x+v[NMS_X],y+v[NMS_Y],z+v[NMS_Z]);
}

Vector  Vector::operator-(Vector const &v)
{
	return Vector(x-v[NMS_X],y-v[NMS_Y],z-v[NMS_Z]);
}

//Cross Product
Vector  Vector::operator%(Vector const &v)
{
	return Vector(y*v[NMS_Z]-z*v[NMS_Y],z*v[NMS_X]-x*v[NMS_Z],x*v[NMS_Y]-y*v[NMS_X]);
}
	  

float Vector::operator*(Vector const &v)
{
	return this->x * v[NMS_X] +
		   this->y * v[NMS_Y] +
		   this->z * v[NMS_Z];
}

float   Vector::sqmagnitude()
{
	return ((*this)*(*this));
}

float Vector::magnitude()
{
	return sqrt((*this)*(*this));
}


Vector  Vector::normal()
{
   float m = this->magnitude();
			
			if(m == 0)
				//TO BE HANDLED CORRECTLY
				throw 0;

			// 1/|v|*v
			return (*this)*(1/m);
}