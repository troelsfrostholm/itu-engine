#ifndef POINT_H__
#define POINT_H__



#define NMS_X 1
#define NMS_Y 2
#define NMS_Z 3
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
	   __declspec(dllexport) Point  Point::operator+(const Vector &v);
	   __declspec(dllexport) Point  Point::operator-(const Vector &v);
	   __declspec(dllexport) Vector Point::operator-(const Point &p);
	    // Comparison
	   __declspec(dllexport) bool   Point::operator==(const Point &p);
	   __declspec(dllexport) bool   Point::operator!=(const Point &p);

	  //Matrix multiplication
	   __declspec(dllexport) friend   Point operator *(Point& v,const Matrix &m);
	   __declspec(dllexport) Point&   operator *= (const Matrix &m);
};
#endif