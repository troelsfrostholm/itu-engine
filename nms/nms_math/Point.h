class Point
{
  friend class Vector;
  private:
	float x;
	float y;
	float z;

  public:

	  //CONSTRUCTOR
	  Point();
	  Point(float x, float y, float z);

	  //DECONSTRUCTOR
	  ~Point(){};

	  //OPERATORS
	  Point  Point::operator+(Vector &v);
	  Point  Point::operator-(Vector &v);
	  Vector Point::operator-(Point &p);
	    // Comparison
	  int   Point::operator==(Point &p);
	  int   Point::operator!=(Point &p);
};