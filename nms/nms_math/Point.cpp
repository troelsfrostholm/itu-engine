#include "Point.h"
#include "Vector.h"
#include "Matrix.h"



float& Point::operator[](int pos)
{
	switch (pos){
		case NMS_X:return this->x;
			   break;
	    case NMS_Y:return this->y;
			   break;
		case NMS_Z:return this->z;
			   break;
	    case W:return this->w;
			   break;
		default:
			//Need to fix it
			throw 0;
	}
}

float const&  Point::operator[](int pos) const
{
	switch (pos){
		case NMS_X:return this->x;
			   break;
	    case NMS_Y:return this->y;
			   break;
		case NMS_Z:return this->z;
			   break;
	    case W:return this->w;
			   break;
		default:
			//Need to fix it
			throw 0;
	}
}


Point::Point(){x=y=z=0;w=1;};
Point::Point(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=1;
}

bool Point::operator!=(const Point &p)
{
   return this->x != p[NMS_X] &&
	      this->y != p[NMS_Y] &&
	      this->z != p[NMS_Z];
}

bool Point::operator==(const Point &p)
{
   return this->x == p[NMS_X] &&
	      this->y == p[NMS_Y] &&
	      this->z == p[NMS_Z];
}

Point Point::operator +(const Vector &v)
{
	return Point(this->x+v[NMS_X],this->y+v[NMS_Y],this->z+v[NMS_Z]);
}

Vector Point::operator -(const Point &p)
{
	return Vector(this->x-p[NMS_X],this->y-p[NMS_Y],this->z-p[NMS_Z]);
}

Point Point::operator -(const Vector &v)
{
	return Point(this->x-v[NMS_X],this->y-v[NMS_Y],this->z-v[NMS_Z]);
}

Point& Point::operator *= (const Matrix &m)
{
	if (4!=m.getRowL())
	{
		cerr<<"The dimension of the row of the matrix do not match for the * operator, it should be 4!\n";
	   //TO BE FIXED
      throw 0;
	}
	Point temp=Point();
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

Point operator * (Point& p,const Matrix &m)
{
	Point temp = p;
    temp *= m;
    return temp;
}