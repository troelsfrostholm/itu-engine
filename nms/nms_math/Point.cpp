#include "Point.h"
#include "Vector.h"
#include "Matrix.h"



float& Point::operator[](int pos)
{
	switch (pos){
		case X:return this->x;
			   break;
	    case Y:return this->y;
			   break;
		case Z:return this->z;
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
		case X:return this->x;
			   break;
	    case Y:return this->y;
			   break;
		case Z:return this->z;
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

int Point::operator!=(Point &p)
{
   return this->x != p[X] &&
	      this->y != p[Y] &&
	      this->z != p[Z];
}

int Point::operator==(Point &p)
{
   return this->x == p[X] &&
	      this->y == p[Y] &&
	      this->z == p[Z];
}

Point Point::operator +(Vector &v)
{
	return Point(this->x+v[X],this->y+v[Y],this->z+v[Z]);
}

Vector Point::operator -(Point &p)
{
	return Vector(this->x-p[X],this->y-p[Y],this->z-p[Z]);
}

Point Point::operator -(Vector &v)
{
	return Point(this->x-v[X],this->y-v[Y],this->z-v[Z]);
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