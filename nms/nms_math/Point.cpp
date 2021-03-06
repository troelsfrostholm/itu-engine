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
	    case NMS_W:return this->w;
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
	    case NMS_W:return this->w;
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

bool operator!=(const Point &p1, const Point &p2)
{
   return p1[NMS_X] != p2[NMS_X] &&
	      p1[NMS_Y] != p2[NMS_Y] &&
	      p1[NMS_Z] != p2[NMS_Z];
}

bool operator==(const Point &p1, const Point &p2)
{
   return p1[NMS_X] == p2[NMS_X] &&
	      p1[NMS_Y] == p2[NMS_Y] &&
	      p1[NMS_Z] == p2[NMS_Z];
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

ostream& operator<<(ostream& output, const Point& p) {
		return output<<p[NMS_X]<<' '<<p[NMS_Y]<<' '<<p[NMS_Z]<<' '<<p[NMS_W]<<' '<<'\n';
}