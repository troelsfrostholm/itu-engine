#ifndef POINT_H__
#define POINT_H__

#include <fstream>

using namespace std;

#define NMS_X 1
#define NMS_Y 2
#define NMS_Z 3
#define W 4

class __declspec(dllexport) Point
{
  friend class Vector;
  friend class Matrix;
  protected:
	float x;
	float y;
	float z;
	float w;

  public:

	  //CONSTRUCTOR
	    Point();
	    Point(float x, float y, float z);

	  //DECONSTRUCTOR
	    ~Point(){};

	  //OPERATORS
	    float&  Point::operator[](int pos);
	    float const&  Point::operator[](int pos) const;
	    Point  Point::operator+(const Vector &v);
	    Point  Point::operator-(const Vector &v);
	    Vector Point::operator-(const Point &p);
	    // Comparison
	    friend  bool  Point::operator==(const Point &p1, const Point &p2);
	    friend  bool  Point::operator!=(const Point &p1, const Point &p2);

	  //Matrix multiplication
	    friend   Point operator *(Point& v,const Matrix &m);
	    Point&   operator *= (const Matrix &m);

	    friend  ostream& operator<<(ostream& output, const Point& p);
};
#endif