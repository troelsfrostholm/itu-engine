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
	   __declspec(dllexport) Point();
	   __declspec(dllexport) Point(float x, float y, float z);

	  //DECONSTRUCTOR
	   __declspec(dllexport) ~Point(){};

	  //OPERATORS
	   __declspec(dllexport) float&  Point::operator[](int pos);
	   __declspec(dllexport) float const&  Point::operator[](int pos) const;
	   __declspec(dllexport) Point  Point::operator+(Vector &v);
	   __declspec(dllexport) Point  Point::operator-(Vector &v);
	   __declspec(dllexport) Vector Point::operator-(Point &p);
	    // Comparison
	   __declspec(dllexport) int   Point::operator==(Point &p);
	   __declspec(dllexport) int   Point::operator!=(Point &p);

	  //Matrix multiplication
	   __declspec(dllexport) friend   Point operator *(Point& v,const Matrix &m);
	   __declspec(dllexport) Point&   operator *= (const Matrix &m);
};
#endif