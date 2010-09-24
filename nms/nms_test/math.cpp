#include "Point.h"
#include "Vector.h"
#include "Matrix.h"
#include "approx.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace std;
using namespace nms::math;

BOOST_AUTO_TEST_SUITE( approx );

BOOST_AUTO_TEST_CASE( approx_approximates )
{
	BOOST_CHECK( approximates(1.01f, 1.00f, 0.02f) );
	BOOST_CHECK( !approximates(1.01f, 1.00f, 0.005f) );
	BOOST_CHECK( approximates( Vector(1.0f, 1.0f, 1.0f), 
		                       Vector(1.01f, 1.01f, 1.0f), 
							   0.02f ) );
	BOOST_CHECK( !approximates( Vector(1.0f, 1.0f, 1.0f), 
		                        Vector(1.01f, 1.01f, 1.0f), 
								0.005f ) );
	//ToDo: Check approximation for matrices too..
}
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE( point );

BOOST_AUTO_TEST_CASE( point_construction_and_lookup )
{
	//Empty constructor
	Point p = Point();
	BOOST_CHECK_EQUAL( p[1], 0 );
	BOOST_CHECK_EQUAL( p[2], 0 );
	BOOST_CHECK_EQUAL( p[3], 0 );
	BOOST_CHECK_EQUAL( p[4], 1 );

	//Construct with coordinates
	Point q = Point(2, 3, 4);
	BOOST_CHECK_EQUAL( q[1], 2 );
	BOOST_CHECK_EQUAL( q[2], 3 );
	BOOST_CHECK_EQUAL( q[3], 4 );
	BOOST_CHECK_EQUAL( q[4], 1 );

	//Todo: Invalid lookup should throw errors. q[0], q[4], q[-1]..
	//How to test that with boost unit?

	//Test deconstructor?
}

BOOST_AUTO_TEST_CASE( point_assignment )
{
	Point p = Point();

	for(int i=1; i<4; i++) {
		p[i] = (float) -i;
		BOOST_CHECK_EQUAL(p[i], -i);
	}
}

BOOST_AUTO_TEST_CASE( point_comparison )
{
	Point p = Point(1, 2, 3);
	Point q = Point(1, 2, 3);
	Point r = Point(2, 3, 4);
	
	BOOST_CHECK( p == p );
	BOOST_CHECK( p == q );
	BOOST_CHECK( !(p == r) );
	BOOST_CHECK( p != r );
}

BOOST_AUTO_TEST_CASE( point_arithmetics )
{
	Point p = Point(1, 2, 3);
	Point q = Point(2, 3, 4);
	Vector v = Vector(2, 3, 4);

	BOOST_CHECK_EQUAL(q - p, Vector(1, 1, 1));
	BOOST_CHECK_EQUAL(p + v, Point(3, 5, 7));
	BOOST_CHECK_EQUAL(p - v, Point(-1, -1, -1));
}


BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE( vector );

BOOST_AUTO_TEST_CASE( vector_construction_and_lookup )
{
	//Empty constructor
	Vector p = Vector();
	BOOST_CHECK_EQUAL( p[1], 0 );
	BOOST_CHECK_EQUAL( p[2], 0 );
	BOOST_CHECK_EQUAL( p[3], 0 );
	BOOST_CHECK_EQUAL( p[4], 0 );

	//Construct with three coordinates
	Vector q = Vector(2, 3, 4);
	BOOST_CHECK_EQUAL( q[1], 2 );
	BOOST_CHECK_EQUAL( q[2], 3 );
	BOOST_CHECK_EQUAL( q[3], 4 );
	BOOST_CHECK_EQUAL( q[4], 0 );
	
	//Construct with four coordinates
	Vector r = Vector(2, 3, 4, 1);
	BOOST_CHECK_EQUAL( r[1], 2 );
	BOOST_CHECK_EQUAL( r[2], 3 );
	BOOST_CHECK_EQUAL( r[3], 4 );
	BOOST_CHECK_EQUAL( r[4], 1 );
}

BOOST_AUTO_TEST_CASE( vector_arithmetics )
{
	Vector a = Vector(1, 2, 3);
	Vector b = Vector(2, 3, 4);

	//addition
	BOOST_CHECK_EQUAL( a+b, Vector(3, 5, 7) );
	BOOST_CHECK_EQUAL( b+a, Vector(3, 5, 7) );

	//subtraction
	BOOST_CHECK_EQUAL( a-b, Vector(-1, -1, -1) );
	BOOST_CHECK_EQUAL( b-a, Vector(1, 1, 1) );

	//scalar multiplication
	BOOST_CHECK_EQUAL( 3*a, Vector(3, 6, 9) );
	BOOST_CHECK_EQUAL( a*3, Vector(3, 6, 9) );

	//dot product
	BOOST_CHECK_EQUAL( a*b, 20 );
	BOOST_CHECK_EQUAL( b*a, 20 );

	//cross product
	BOOST_CHECK_EQUAL( a%b, Vector(-1, 2, -1));
}

BOOST_AUTO_TEST_CASE( vector_arithmetics_assignment )
{
	Vector c;
	
	//Scalar product and division
	c = Vector(-1, 2, 3);
	c *= 3;
	BOOST_CHECK_EQUAL( c, Vector(-3, 6, 9) );
	c /= 3;
	BOOST_CHECK_EQUAL( c, Vector(-1, 2, 3) );

	//addition
	c = Vector(1, 2, 3);
	c += Vector(2, 3, 4);
	BOOST_CHECK_EQUAL( c, Vector(3, 5, 7) );

	//subtraction
	c = Vector(1, 2, 3);
	c -= Vector(2, 3, 4);
	BOOST_CHECK_EQUAL( c, Vector(-1, -1, -1) );
}

BOOST_AUTO_TEST_CASE( vector_properties )
{
	Vector v = Vector(1, 2, 3);

	//squared magnitude
	float smgt = v.sqmagnitude();
	BOOST_CHECK_EQUAL(smgt, 14);
	BOOST_CHECK_EQUAL(smgt, v*v);

	//magnitude
	BOOST_CHECK_EQUAL(v.magnitude(), sqrt(14.0f));

	//normalized
	float e = 0.000001f; //max allowed error;
	Vector normal = v.normal();
	Vector expected = Vector( 0.267261f,  
		                      0.534522f,  
							  0.801784f ); //expected vector calculated with python's numpy and truncated to 6th decimal. 
	
	BOOST_CHECK( approximates(normal, expected, e) );

	//normalized vector is parallel with original vector
	BOOST_CHECK( approximates( (v%normal), Vector(0.0f, 0.0f, 0.0f), e ) );

	//normalized vector has magnitude of 1
	BOOST_CHECK( approximates(normal.magnitude(), 1.0f, e) );

}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE( matrix );

BOOST_AUTO_TEST_CASE( matrix_construction_and_lookup )
{
	Matrix m = Matrix();
	for(int i=1; i<=4; i++) {
		for(int j=1; j<=4; j++) {
			if(i==j)
				BOOST_CHECK_EQUAL(m(i, j), 1);
			else
				BOOST_CHECK_EQUAL(m(i, j), 0);
		}
	}

	//Should test other constructors too
}

BOOST_AUTO_TEST_CASE( matrix_invertion )
{
	Matrix identity = Matrix();
	cout << identity << endl;
	Matrix identity_inverse = !identity;
	cout << identity_inverse << endl;
	BOOST_CHECK_EQUAL(identity, identity_inverse);
}

BOOST_AUTO_TEST_SUITE_END();