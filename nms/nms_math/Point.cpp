#include "stdafx.h"
#include "Point.h"
#include "Vector.h"




Point::Point()
{
	x=0;
	y=0;
	z=0;
}

float Point::operator[](int pos)
{
	switch (pos){
		case 0:return this->x;
			   break;
	    case 1:return this->y;
			   break;
		case 2:return this->z;
			   break;
		default:
			//Need to fix it
			throw 0;
	}
}

Point::Point(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
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