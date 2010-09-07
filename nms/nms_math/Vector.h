
public class Vector : public Point
{
  public:
	  
	  //CONSTRUCTORS
	  Vector():Point();
	  Vector(float x,float y,float z):Point(x,y,z);


	  //OPERATORS
	  float   Vector::operator[](int pos);
	  void    Vector::operator[]=(int pos);
	  Vector  Vector::operator+(Vector const &v);
	  Vector  Vector::operator-(Vector const &v);
	  //Dot Product
	  float   Vector::operator*(Vector const &v);
	  //Cross Product
	  Vector  Vector::operator%(Vector const &v);

	  Vector  Vector::normal();
	  float   Vector::magnitude();
	  float   Vector::sqmagnitude();
}