#ifndef POINT_H__
#define POINT_H__



#define X 1
#define Y 2
#define Z 3
#define W 4

class Point
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
	  Point  Point::operator+(Vector &v);
	  Point  Point::operator-(Vector &v);
	  Vector Point::operator-(Point &p);
	    // Comparison
	  int   Point::operator==(Point &p);
	  int   Point::operator!=(Point &p);

	  //Matrix multiplication
	  friend   Point operator *(Point& v,const Matrix &m);
	  Point&   operator *= (const Matrix &m);
};
#endif