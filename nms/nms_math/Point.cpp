#include "stdafx.h"
#include "Point.h"
#include "Vector.h"


Point::Point()
{
	x=0;
	y=0;
	z=0;
}

Point::Point(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}

int Point::operator!=(Point &p)
{
   return this->x != p.x &&
	      this->y != p.y &&
	      this->z != p.z;
}

int Point::operator==(Point &p)
{
   return this->x == p.x &&
	      this->y == p.y &&
	      this->z == p.z;
}

Point Point::operator +(Vector &v)
{
	return Point(this->x+v.x,this->y+v.y,this->z+v.z);
}

Vector Point::operator -(Point &p)
{
	return Vector(this->x-p.x,this->y-p.y,this->z-p.z);
}

Point Point::operator -(Vector &v)
{
	return Point(this->x-v.x,this->y-v.y,this->z-v.z);
}