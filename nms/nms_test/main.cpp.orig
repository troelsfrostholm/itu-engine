#include "Point.h"
#include "Vector.h"
#include "Matrix.h"
#define BOOST_TEST_MODULE MathTest

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cmath>

using namespace std;

BOOST_AUTO_TEST_SUITE( point );

BOOST_AUTO_TEST_CASE( point_construction_and_lookup )
{
	//Empty constructor
	Point p = Point();
	BOOST_CHECK_EQUAL( p[1], 0 );
	BOOST_CHECK_EQUAL( p[2], 0 );
	BOOST_CHECK_EQUAL( p[3], 0 );

	//Construct with coordinates
	Point q = Point(2, 3, 4);
	BOOST_CHECK_EQUAL( q[1], 2 );
	BOOST_CHECK_EQUAL( q[2], 3 );
	BOOST_CHECK_EQUAL( q[3], 4 );

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
	Vector vm = Vector(2,2,2);

	BOOST_CHECK_EQUAL(q - p, Vector(1, 1, 1));
	BOOST_CHECK_EQUAL(p + v, Point(3, 5, 7));
	BOOST_CHECK_EQUAL(p - v, Point(-1, -1, -1));
	//BOOST_CHECK_EQUAL(vm.magnitude(),sqrt(12));
}


BOOST_AUTO_TEST_SUITE_END();