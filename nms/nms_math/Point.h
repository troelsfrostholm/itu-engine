
public class Point
{
  friend class Vector;
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
	  int operator==( Point);
	  int operator!=( Point);
}